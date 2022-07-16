/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/Properties/Settings.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/DbgRenderer.h>
#include <osre/Threading/SystemTask.h>

#include "OGLRenderer/OGLRenderEventHandler.h"
#include "VulkanRenderer/VlkRenderEventHandler.h"
// clang-format off
#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#endif

// clang-format on
namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Threading;
using namespace ::OSRE::Properties;

static const c8 *Tag = "RenderBackendService";

static constexpr c8 OGL_API[] = "opengl";
static constexpr c8 Vulkan_API[] = "vulkan";
static constexpr i32 IdxNotFound = -1;

static i32 hasPass(const c8 *id, const ::CPPCore::TArray<PassData *> &passDataArray) {
    if (nullptr == id) {
        return IdxNotFound;
    }

    for (ui32 i = 0; i < passDataArray.size(); ++i) {
        if (0 == strncmp(passDataArray[i]->m_id, id, strlen(id))) {
            return i;
        }
    }
    return IdxNotFound;
}

static i32 hasBatch(const c8 *id, const ::CPPCore::TArray<RenderBatchData*> &batchDataArray) {
    if (nullptr == id) {
        return IdxNotFound;
    }

    for (ui32 i = 0; i < batchDataArray.size(); ++i) {
        if (0 == strncmp(batchDataArray[i]->m_id, id, strlen(id))) {
            return i;
        }
    }
    return IdxNotFound;
}

RenderBackendService::RenderBackendService() :
        AbstractService("renderbackend/renderbackendserver"),
        m_renderTaskPtr(),
        m_settings(nullptr),
        mViewport(),
        m_ownsSettingsConfig(false),
        m_frameCreated(false),
        m_renderFrame(&m_frames[0]),
        m_submitFrame(&m_frames[1]),
        m_dirty(false),
        m_passes(),
        m_currentPass(nullptr),
        m_currentBatch(nullptr),
        mPipelines() {
    // empty
}

RenderBackendService::~RenderBackendService() {
    if (m_ownsSettingsConfig) {
        delete m_settings;
        m_settings = nullptr;
    }

    for (ui32 i = 0; i < mPipelines.size(); ++i) {
        delete mPipelines[i];
    }
    mPipelines.clear();

    for (ui32 i = 0; i < m_passes.size(); ++i) {
        delete m_passes[i];
    }
    m_passes.clear();
}

bool RenderBackendService::onOpen() {
    if (nullptr == m_settings) {
        m_settings = new Settings;
        m_ownsSettingsConfig = true;
    }

    
    // Spawn the thread for our render task
    if (!m_renderTaskPtr.isValid()) {
        m_renderTaskPtr.init(SystemTask::create("render_task"));
    }

    // Run the render task
    bool ok = m_renderTaskPtr->start(nullptr);
    if (!ok) {
        osre_error(Tag, "Cannot run render task.");
        return ok;
    }

    // Create render event handler for back-end
    const String api = m_settings->get(Settings::RenderAPI).getString();
    if (api == OGL_API) {
        m_renderTaskPtr->attachEventHandler(new OGLRenderEventHandler);
    } else if (api == Vulkan_API) {
        m_renderTaskPtr->attachEventHandler(new VlkRenderEventHandler);
    } else {
        osre_error(Tag, "Requested render-api unknown: " + api);
        ok = false;
    }

    // Create the debug renderer instance
    if (!Scene::DbgRenderer::create(this)) {
        osre_error(Tag, "Cannot create Debug renderer");
        ok = false;
    }

    return ok;
}

bool RenderBackendService::onClose() {
    if (!m_renderTaskPtr.isValid()) {
        return false;
    }

    if (!Scene::DbgRenderer::destroy()) {
        osre_error(Tag, "Cannot destroy Debug renderer");
    }
    if (m_renderTaskPtr->isRunning()) {
        m_renderTaskPtr->detachEventHandler();
        m_renderTaskPtr->stop();
    }

    return true;
}

bool RenderBackendService::onUpdate() {
    if (!m_renderTaskPtr.isValid()) {
        return false;
    }

    if (!m_frameCreated) {
        initPasses();
        m_frameCreated = true;
    }

    commitNextFrame();

    // Synchronizing event with render back-end
    auto result = m_renderTaskPtr->sendEvent(&OnRenderFrameEvent, nullptr);
    m_renderTaskPtr->awaitUpdate();

    return result;
}

void RenderBackendService::setSettings(const Settings *config, bool moveOwnership) {
    if (m_ownsSettingsConfig && m_settings != nullptr) {
        delete m_settings;
        m_settings = nullptr;
        m_ownsSettingsConfig = false;
    }
    m_settings = config;
    m_ownsSettingsConfig = moveOwnership;
}

const Settings *RenderBackendService::getSettings() const {
    return m_settings;
}

void RenderBackendService::initPasses() {
    if (!m_renderTaskPtr.isValid()) {
        return;
    }

    InitPassesEventData *data = new InitPassesEventData;
    m_submitFrame->init(m_passes);
    data->m_frame = m_submitFrame;

    m_renderTaskPtr->sendEvent(&OnInitPassesEvent, data);
}

void RenderBackendService::commitNextFrame() {
    if (!m_renderTaskPtr.isValid()) {
        return;
    }

    CommitFrameEventData *data = new CommitFrameEventData;
    data->m_frame = m_submitFrame;
    for (ui32 i = 0; i < m_passes.size(); ++i) {
        PassData *currentPass = m_passes[i];
        for (ui32 j = 0; j < currentPass->m_geoBatches.size(); ++j) {
            RenderBatchData *currentBatch = currentPass->m_geoBatches[j];
            if (currentBatch->m_dirtyFlag & RenderBatchData::MatrixBufferDirty) {
                FrameSubmitCmd *cmd = m_submitFrame->enqueue();
                cmd->m_passId = currentPass->m_id;
                cmd->m_batchId = currentBatch->m_id;
                cmd->m_updateFlags |= (ui32) FrameSubmitCmd::UpdateMatrixes;
                cmd->m_size = sizeof(MatrixBuffer);
                cmd->m_data = new c8[cmd->m_size];
                ::memcpy(cmd->m_data, &currentBatch->m_matrixBuffer, cmd->m_size);
            } 
            
            if (currentBatch->m_dirtyFlag & RenderBatchData::UniformBufferDirty) {
                UniformBuffer &uniformBuffer = data->m_frame->m_uniforBuffers[i];
                for (ui32 k = 0; k < currentBatch->m_uniforms.size(); ++k) {
                    FrameSubmitCmd *cmd = m_submitFrame->enqueue();
                    cmd->m_passId = currentPass->m_id;
                    cmd->m_batchId = currentBatch->m_id;
                    cmd->m_updateFlags |= (ui32)FrameSubmitCmd::UpdateUniforms;
                    UniformVar *var = currentBatch->m_uniforms[k];
                    if (nullptr == var) {
                        continue;
                    }

                    uniformBuffer.writeVar(var);

                    // todo: replace by uniform buffer.
                    cmd->m_size = var->getSize();
                    cmd->m_data = new c8[cmd->m_size];
                    size_t offset = 0;
                    cmd->m_data[offset] = var->m_name.size() > 255 ? 255 : static_cast<c8>(var->m_name.size());
                    ++offset;
                    ::memcpy(&cmd->m_data[offset], var->m_name.c_str(), var->m_name.size());
                    offset += var->m_name.size();
                    ::memcpy(&cmd->m_data[offset], var->m_data.getData(), var->m_data.m_size);
                }
            } 
            
            if (currentBatch->m_dirtyFlag & RenderBatchData::MeshUpdateDirty) {
                for (ui32 k = 0; k < currentBatch->m_updateMeshArray.size(); ++k) {
                    FrameSubmitCmd *cmd = m_submitFrame->enqueue();
                    cmd->m_passId = currentPass->m_id;
                    cmd->m_batchId = currentBatch->m_id;
                    cmd->m_updateFlags |= (ui32)FrameSubmitCmd::UpdateBuffer;
                    Mesh *currentMesh = currentBatch->m_updateMeshArray[k];
                    cmd->m_meshId = currentMesh->getId();
                    cmd->m_size = currentMesh->getVertexBuffer()->getSize();
                    cmd->m_data = new c8[cmd->m_size];
                    ::memcpy(cmd->m_data, currentMesh->getVertexBuffer()->getData(), cmd->m_size);
                }
            } 
            if (currentBatch->m_dirtyFlag & RenderBatchData::MeshDirty) {
                FrameSubmitCmd *cmd = m_submitFrame->enqueue();
                PassData *pd = new PassData(currentPass->m_id, nullptr);
                pd->m_geoBatches.add(currentBatch);
                cmd->m_updatedPasses.add(pd);
                cmd->m_updateFlags |= (ui32)FrameSubmitCmd::AddRenderData;
            }

            currentBatch->m_dirtyFlag = 0;
        }
    }

    data->m_frame = m_submitFrame;
    std::swap(m_submitFrame, m_renderFrame);

    m_renderTaskPtr->sendEvent(&OnCommitFrameEvent, data);
}

void RenderBackendService::sendEvent(const Event *ev, const EventData *eventData) {
    osre_assert(ev != nullptr);

    if (m_renderTaskPtr.isValid()) {
        m_renderTaskPtr->sendEvent(ev, eventData);
    }
}

Pipeline *RenderBackendService::createDefaultPipeline() {
    auto *defaultPipeline = findPipeline(DefaultPipelines::get_Pipeline_Default());
    if (defaultPipeline != nullptr) {
        return defaultPipeline;
    }

    Pipeline *pipeline = new Pipeline(DefaultPipelines::get_Pipeline_Default());
    RenderPass *renderPass = RenderPassFactory::create(RenderPassId);
    CullState cullState(CullState::CullMode::CCW, CullState::CullFace::Back);
    renderPass->setCullState(cullState);
    pipeline->addPass(renderPass);
    mPipelines.add(pipeline);

    return pipeline;
}

Pipeline *RenderBackendService::createPipeline(const String &name) {
    Pipeline *p = findPipeline(name);
    if (nullptr == p) {
        p = new Pipeline(name);
        mPipelines.add(p);
    }

    return p;
}

Pipeline *RenderBackendService::findPipeline(const String &name) {
    if (name.empty()) {
        return nullptr;
    }

    RenderBackend::Pipeline *pl = nullptr;
    for (ui32 i = 0; i < mPipelines.size(); ++i) {
        if (mPipelines[i]->getName() == name) {
            pl = mPipelines[i];
            break;
        }
    }

    return pl;
}

bool RenderBackendService::destroyPipeline(const String &name) {
    if (name.empty()) {
        return false;
    }

    for (ui32 i = 0; i < mPipelines.size(); ++i) {
        if (mPipelines[i]->getName() == name) {
            mPipelines.remove(i);
            return true;
        }
    }

    return false;
}

PassData *RenderBackendService::getPassById(const c8 *id) const {
    if (nullptr == id) {
        return nullptr;
    }

    if (nullptr != m_currentPass) {
        if (0 == ::strncmp(m_currentPass->m_id, id, strlen(id))) {
            return m_currentPass;
        }
    }

    for (ui32 i = 0; i < m_passes.size(); ++i) {
        if (0 == ::strncmp(m_passes[i]->m_id, id, strlen(id))) {
            return m_passes[i];
        }
    }

    return nullptr;
}

PassData *RenderBackendService::beginPass(const c8 *id) {
    if (nullptr != m_currentPass) {
        osre_warn(Tag, "Pass recording already active.");
        return nullptr;
    }

    m_currentPass = getPassById(id);
    if (nullptr == m_currentPass) {
        m_currentPass = new PassData(id, nullptr);
    }
    m_dirty = true;

    return m_currentPass;
}

RenderBatchData *RenderBackendService::beginRenderBatch(const c8 *id) {
    if (nullptr == m_currentPass) {
        osre_warn(Tag, "Pass recording not active.");
        return nullptr;
    }

    m_currentBatch = m_currentPass->getBatchById(id);
    if (nullptr == m_currentBatch) {
        m_currentBatch = new RenderBatchData(id);
    }

    return m_currentBatch;
}

void RenderBackendService::setRenderTarget(FrameBuffer *fb) {
    if (m_currentPass == nullptr) {
        osre_warn(Tag, "No active pass, cannot add render target.");
        return;
    }
    
    if (fb == nullptr) {
        osre_error(Tag, "Framebuffer is nullptr, aborted.");
        return;
    }

    m_currentPass->m_renderTarget = fb;
}

void RenderBackendService::setMatrix(MatrixType type, const glm::mat4 &m) {
    if (nullptr == m_currentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    switch (type) {
        case MatrixType::Model:
            m_currentBatch->m_matrixBuffer.m_model = m;
            m_currentBatch->m_dirtyFlag |= RenderBatchData::MatrixBufferDirty;
            break;
        case MatrixType::View:
            m_currentBatch->m_matrixBuffer.m_view = m;
            m_currentBatch->m_dirtyFlag |= RenderBatchData::MatrixBufferDirty;
            break;
        case MatrixType::Projection:
            m_currentBatch->m_matrixBuffer.m_proj = m;
            m_currentBatch->m_dirtyFlag |= RenderBatchData::MatrixBufferDirty;
            break;
        default:
            break;
    }
}

void RenderBackendService::setMatrix(const String &name, const glm::mat4 &matrix) {
    if (nullptr == m_currentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    UniformVar *var = m_currentBatch->getVarByName(name.c_str());
    if (nullptr == var) {
        var = UniformVar::create(name, ParameterType::PT_Mat4);
        m_currentBatch->m_uniforms.add(var);
    }

    m_currentBatch->m_dirtyFlag |= RenderBatchData::UniformBufferDirty;
    ::memcpy(var->m_data.m_data, glm::value_ptr(matrix), sizeof(glm::mat4));
}

void RenderBackendService::setUniform(UniformVar *var) {
    if (nullptr == var) {
        osre_error(Tag, "No active batch.");
        return;
    }

    if (nullptr != m_currentBatch) {
        m_currentBatch->m_uniforms.add(var);
        m_currentBatch->m_dirtyFlag |= RenderBatchData::UniformBufferDirty;
    }
}

void RenderBackendService::setMatrixArray(const String &name, ui32 numMat, const glm::mat4 *matrixArray) {
    if (nullptr == m_currentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    UniformVar *var = m_currentBatch->getVarByName(name.c_str());
    if (nullptr == var) {
        var = UniformVar::create(name, ParameterType::PT_Mat4Array, numMat);
        m_currentBatch->m_uniforms.add(var);
    }

    ::memcpy(var->m_data.m_data, glm::value_ptr(matrixArray[0]), sizeof(glm::mat4) * numMat);
    m_currentBatch->m_dirtyFlag |= RenderBatchData::UniformBufferDirty;
}

void RenderBackendService::addMesh(Mesh *mesh, ui32 numInstances) {
    if (nullptr == mesh) {
        osre_debug(Tag, "Pointer to geometry is nullptr.");
        return;
    }

    if (nullptr == m_currentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    MeshEntry *entry = new MeshEntry;
    entry->mMeshArray.add(mesh);
    entry->numInstances = numInstances;
    m_currentBatch->m_meshArray.add(entry);
    m_currentBatch->m_dirtyFlag |= RenderBatchData::MeshDirty;
}

void RenderBackendService::addMesh(const CPPCore::TArray<Mesh *> &geoArray, ui32 numInstances) {
    if (nullptr == m_currentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    MeshEntry *entry = new MeshEntry;
    entry->numInstances = numInstances;
    entry->mMeshArray.add(&geoArray[0], geoArray.size());
    m_currentBatch->m_meshArray.add(entry);
    m_currentBatch->m_dirtyFlag |= RenderBatchData::MeshDirty;
}

void RenderBackendService::updateMesh(Mesh *mesh) {
    if (nullptr == m_currentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }
    m_currentBatch->m_updateMeshArray.add(mesh);
    m_currentBatch->m_dirtyFlag |= RenderBatchData::MeshUpdateDirty;
}

bool RenderBackendService::endRenderBatch() {
    if (nullptr == m_currentBatch) {
        return false;
    }

    if (nullptr == m_currentPass) {
        m_currentPass = new PassData("defaultPass", nullptr);
    }

    if (-1 == hasBatch(m_currentBatch->m_id, m_currentPass->m_geoBatches)) {
        m_currentPass->m_geoBatches.add(m_currentBatch);
    }

    m_currentBatch = nullptr;

    return true;
}

bool RenderBackendService::endPass() {
    if (nullptr == m_currentPass) {
        return false;
    }

    if (-1 == hasPass(m_currentPass->m_id, m_passes)) {
        m_passes.add(m_currentPass);
    }
    m_currentPass = nullptr;

    return true;
}

void RenderBackendService::clearPasses() {
    m_currentPass = nullptr;

    for (ui32 i = 0; i < m_passes.size(); ++i) {
        delete m_passes[i];
    }
    m_passes.clear();
    m_frameCreated = false;
}

void RenderBackendService::attachView() {    
    // empty
}

void RenderBackendService::resize(ui32 x, ui32 y, ui32 w, ui32 h) {
    if (mBehaviour.ResizeViewport) {
        ResizeEventData *data = new ResizeEventData(x, y, w, h);
        m_renderTaskPtr->sendEvent(&OnResizeEvent, data);
    }
}

void RenderBackendService::focusLost() {
    // todo: implement me
}

void RenderBackendService::syncRenderThread() {
    // Synchronizing event with render back-end
    auto result = m_renderTaskPtr->sendEvent(&OnRenderFrameEvent, nullptr);
    if(!result) {
        osre_debug(Tag, "Error while requesting next frame.");
    }
    m_renderTaskPtr->awaitUpdate();
}

void RenderBackendService::setViewport( ui32 x, ui32 y, ui32 w, ui32 h ) {
    mViewport.m_x = x;
    mViewport.m_y = y;
    mViewport.m_w = w;
    mViewport.m_h = h;
}

const Viewport &RenderBackendService::getViewport() const {
    return mViewport;
}

} // Namespace RenderBackend
} // Namespace OSRE
