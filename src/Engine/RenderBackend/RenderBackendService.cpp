/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/DbgRenderer.h>
#include <osre/Threading/SystemTask.h>

#include <osre/Debugging/MeshDiagnostic.h>
#include "OGLRenderer/OGLRenderEventHandler.h"
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

static constexpr c8 Tag[] = "RenderBackendService";

static constexpr c8 OGL_API[] = "opengl";
static constexpr c8 Vulkan_API[] = "vulkan";
static constexpr i32 IdxNotFound = -1;

static i32 hasPass(const c8 *id, const ::cppcore::TArray<PassData *> &passDataArray) {
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

static i32 hasBatch(const c8 *id, const ::cppcore::TArray<RenderBatchData*> &batchDataArray) {
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
        mRenderTaskPtr(),
        mSettings(nullptr),
        mViewport(),
        mOwnsSettingsConfig(false),
        mFrameCreated(false),
        mRenderFrame(&mFrames[0]),
        mSubmitFrame(&mFrames[1]),
        mDirty(false),
        mPasses(),
        mPipeline(nullptr),
        mCurrentPass(nullptr),
        mCurrentBatch(nullptr) {
    // empty
}

RenderBackendService::~RenderBackendService() {
    if (mOwnsSettingsConfig) {
        delete mSettings;
    }

    for (ui32 i = 0; i < mPasses.size(); ++i) {
        delete mPasses[i];
    }
    mPasses.clear();
}

bool RenderBackendService::onOpen() {
    if (nullptr == mSettings) {
        mSettings = new Settings;
        mOwnsSettingsConfig = true;
    }

    
    // Spawn the thread for our render task
    if (!mRenderTaskPtr.isValid()) {
        mRenderTaskPtr.init(SystemTask::create("render_task"));
    }

    // Run the render task
    bool ok = mRenderTaskPtr->start(nullptr);
    if (!ok) {
        osre_error(Tag, "Cannot run render task.");
        return ok;
    }

    // Create render event handler for back-end
    const String &api = mSettings->get(Settings::RenderAPI).getString();
    if (api == OGL_API) {
        mRenderTaskPtr->attachEventHandler(new OGLRenderEventHandler);
    } else if (api == Vulkan_API) {
        // todo!
    } else {
        osre_error(Tag, "Requested render-api unknown: " + api);
        ok = false;
    }

    // Create the debug renderer instance
    if (!DbgRenderer::create(this)) {
        osre_error(Tag, "Cannot create Debug renderer");
        ok = false;
    }

    return ok;
}

bool RenderBackendService::onClose() {
    if (!mRenderTaskPtr.isValid()) {
        return false;
    }

    if (!DbgRenderer::destroy()) {
        osre_error(Tag, "Cannot destroy Debug renderer");
    }
    
    if (mRenderTaskPtr->isRunning()) {
        mRenderTaskPtr->detachEventHandler();
        mRenderTaskPtr->stop();
    }

    if (mOwnsSettingsConfig) {
        delete mSettings;
        mSettings = nullptr;
        mOwnsSettingsConfig = false;
    }

    return true;
}

bool RenderBackendService::onUpdate() {
    if (!mRenderTaskPtr.isValid()) {
        return false;
    }

    if (!mFrameCreated) {
        initPasses();
        mFrameCreated = true;
    }

    commitNextFrame();

    // Synchronizing event with render back-end
    auto result = mRenderTaskPtr->sendEvent(&OnRenderFrameEvent, nullptr);
    mRenderTaskPtr->awaitUpdate();

    return result;
}

void RenderBackendService::setSettings(const Settings *config, bool moveOwnership) {
    if (mOwnsSettingsConfig && mSettings != nullptr) {
        delete mSettings;
        mSettings = nullptr;
        mOwnsSettingsConfig = false;
    }
    mSettings = config;
    mOwnsSettingsConfig = moveOwnership;
    setViewport(0, 0, mSettings->getInt(Properties::Settings::WinWidth),  mSettings->getInt(Properties::Settings::WinHeight));
}

const Settings *RenderBackendService::getSettings() const {
    return mSettings;
}

void RenderBackendService::initPasses() {
    if (!mRenderTaskPtr.isValid()) {
        return;
    }

    InitPassesEventData *data = new InitPassesEventData;
    mSubmitFrame->init(mPasses);
    data->m_frame = mSubmitFrame;

    mRenderTaskPtr->sendEvent(&OnInitPassesEvent, data);
}

void RenderBackendService::commitNextFrame() {
    if (!mRenderTaskPtr.isValid()) {
        return;
    }

    CommitFrameEventData *data = new CommitFrameEventData;
    data->m_frame = mSubmitFrame;
    for (ui32 i = 0; i < mPasses.size(); ++i) {
        PassData *currentPass = mPasses[i];
        
        for (ui32 j = 0; j < currentPass->mMeshBatches.size(); ++j) {
            RenderBatchData *currentBatch = currentPass->mMeshBatches[j];
            if (currentBatch->m_dirtyFlag & RenderBatchData::MatrixBufferDirty) {
                printf("matrix_buffer_dirtry\n");
                FrameSubmitCmd *cmd = mSubmitFrame->enqueue();
                currentBatch->m_matrixBuffer.m_view = currentPass->mView;
                currentBatch->m_matrixBuffer.m_proj = currentPass->mProj;
                cmd->m_passId = currentPass->m_id;
                cmd->m_batchId = currentBatch->m_id;
                cmd->m_updateFlags |= (ui32) FrameSubmitCmd::UpdateMatrixes;
                cmd->m_size = sizeof(MatrixBuffer);
                cmd->m_data = new c8[cmd->m_size];
                Debugging::MeshDiagnostic::dump_matrix(currentBatch->m_matrixBuffer.m_model);

                ::memcpy(cmd->m_data, &currentBatch->m_matrixBuffer, cmd->m_size);
            } 
            
            if (currentBatch->m_dirtyFlag & RenderBatchData::UniformBufferDirty) {
                UniformBuffer &uniformBuffer = data->m_frame->m_uniforBuffers[i];
                for (ui32 k = 0; k < currentBatch->m_uniforms.size(); ++k) {
                    FrameSubmitCmd *cmd = mSubmitFrame->enqueue();
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
                    FrameSubmitCmd *cmd = mSubmitFrame->enqueue();
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
                FrameSubmitCmd *cmd = mSubmitFrame->enqueue();
                PassData *pd = new PassData(currentPass->m_id, nullptr);
                pd->mMeshBatches.add(currentBatch);
                cmd->m_updatedPasses.add(pd);
                cmd->m_updateFlags |= (ui32)FrameSubmitCmd::AddRenderData;
            }

            currentBatch->m_dirtyFlag = 0;
        }
    }

    data->m_frame = mSubmitFrame;
    std::swap(mSubmitFrame, mRenderFrame);

    mRenderTaskPtr->sendEvent(&OnCommitFrameEvent, data);
}

void RenderBackendService::sendEvent(const Event *ev, const EventData *eventData) {
    osre_assert(ev != nullptr);

    if (mRenderTaskPtr.isValid()) {
        mRenderTaskPtr->sendEvent(ev, eventData);
    }
}

Pipeline *RenderBackendService::createDefaultPipeline() {
    Pipeline *pipeline = new Pipeline(DefaultPipelines::get_Pipeline_Default());
    RenderPass *renderPass = RenderPassFactory::create(RenderPassId);
    CullState cullState(CullState::CullMode::CCW, CullState::CullFace::Back);
    renderPass->setCullState(cullState);
    renderPass->setViewport(mViewport);
    pipeline->addPass(renderPass);

    return pipeline;
}

void RenderBackendService::setActivePipeline(Pipeline *pipeline) {
    mPipeline = pipeline;
}

PassData *RenderBackendService::getPassById(const c8 *id) const {
    if (nullptr == id) {
        return nullptr;
    }

    if (nullptr != mCurrentPass) {
        if (0 == ::strncmp(mCurrentPass->m_id, id, strlen(id))) {
            return mCurrentPass;
        }
    }

    for (ui32 i = 0; i < mPasses.size(); ++i) {
        if (0 == ::strncmp(mPasses[i]->m_id, id, strlen(id))) {
            return mPasses[i];
        }
    }

    return nullptr;
}

PassData *RenderBackendService::beginPass(const c8 *id) {
    if (nullptr != mCurrentPass) {
        osre_warn(Tag, "Pass recording already active.");
        return nullptr;
    }

    mCurrentPass = getPassById(id);
    if (nullptr == mCurrentPass) {
        mCurrentPass = new PassData(id, nullptr);
        mCurrentPass->mViewport = mViewport;
    }
    mDirty = true;

    return mCurrentPass;
}

RenderBatchData *RenderBackendService::beginRenderBatch(const c8 *id) {
    if (nullptr == mCurrentPass) {
        osre_warn(Tag, "Pass recording not active.");
        return nullptr;
    }

    mCurrentBatch = mCurrentPass->getBatchById(id);
    if (nullptr == mCurrentBatch) {
        mCurrentBatch = new RenderBatchData(id);
    }

    return mCurrentBatch;
}

void RenderBackendService::setRenderTarget(FrameBuffer *fb) {
    if (mCurrentPass == nullptr) {
        osre_warn(Tag, "No active pass, cannot add render target.");
        return;
    }
    
    if (fb == nullptr) {
        osre_error(Tag, "Framebuffer is nullptr, aborted.");
        return;
    }

    mCurrentPass->mRenderTarget = fb;
}

void RenderBackendService::setMatrix(MatrixType type, const glm::mat4 &m) {
    if (nullptr == mCurrentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    switch (type) {
        case MatrixType::Model:
            mCurrentBatch->m_matrixBuffer.m_model = m;
            mCurrentBatch->m_dirtyFlag |= RenderBatchData::MatrixBufferDirty;
            break;
        case MatrixType::View:
            mCurrentPass->mView = m;
            mCurrentBatch->m_matrixBuffer.m_view = m;
            mCurrentBatch->m_dirtyFlag |= RenderBatchData::MatrixBufferDirty;
            break;
        case MatrixType::Projection:
            mCurrentPass->mProj = m;
            mCurrentBatch->m_matrixBuffer.m_proj = m;
            mCurrentBatch->m_dirtyFlag |= RenderBatchData::MatrixBufferDirty;
            break;
        default:
            break;
    }
}

void RenderBackendService::setMatrix(const String &name, const glm::mat4 &matrix) {
    if (nullptr == mCurrentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    UniformVar *var = mCurrentBatch->getVarByName(name.c_str());
    if (nullptr == var) {
        var = UniformVar::create(name, ParameterType::PT_Mat4);
        mCurrentBatch->m_uniforms.add(var);
    }

    mCurrentBatch->m_dirtyFlag |= RenderBatchData::UniformBufferDirty;
    ::memcpy(var->m_data.m_data, glm::value_ptr(matrix), sizeof(glm::mat4));
}

void RenderBackendService::setUniform(UniformVar *var) {
    if (nullptr == var) {
        osre_error(Tag, "No active batch.");
        return;
    }

    if (nullptr != mCurrentBatch) {
        mCurrentBatch->m_uniforms.add(var);
        mCurrentBatch->m_dirtyFlag |= RenderBatchData::UniformBufferDirty;
    }
}

void RenderBackendService::setMatrixArray(const String &name, ui32 numMat, const glm::mat4 *matrixArray) {
    if (nullptr == mCurrentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    UniformVar *var = mCurrentBatch->getVarByName(name.c_str());
    if (nullptr == var) {
        var = UniformVar::create(name, ParameterType::PT_Mat4Array, numMat);
        mCurrentBatch->m_uniforms.add(var);
    }

    ::memcpy(var->m_data.m_data, glm::value_ptr(matrixArray[0]), sizeof(glm::mat4) * numMat);
    mCurrentBatch->m_dirtyFlag |= RenderBatchData::UniformBufferDirty;
}

void RenderBackendService::addMesh(Mesh *mesh, ui32 numInstances) {
    if (mesh == nullptr) {
        osre_debug(Tag, "Pointer to geometry is nullptr.");
        return;
    }

    if (mCurrentBatch == nullptr) {
        osre_error(Tag, "No active batch.");
        return;
    }

    MeshEntry *entry = new MeshEntry;
    entry->mMeshArray.add(mesh);
    entry->numInstances = numInstances;
    mCurrentBatch->m_meshArray.add(entry);
    mCurrentBatch->m_dirtyFlag |= RenderBatchData::MeshDirty;
}

void RenderBackendService::addMesh(const cppcore::TArray<Mesh *> &meshArray, ui32 numInstances) {
    if (mCurrentBatch == nullptr) {
        osre_error(Tag, "No active batch.");
        return;
    }

    if (meshArray.isEmpty()) {
        osre_error(Tag, "Mesh array is empty.");
        return;
    }

    MeshEntry *entry = new MeshEntry;
    entry->numInstances = numInstances;
    entry->mMeshArray.add(&meshArray[0], meshArray.size());
    mCurrentBatch->m_meshArray.add(entry);
    mCurrentBatch->m_dirtyFlag |= RenderBatchData::MeshDirty;
}

void RenderBackendService::updateMesh(Mesh *mesh) {
    if (nullptr == mCurrentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    if (mesh == nullptr) {
        osre_error(Tag, "Mesh is nullptr.");
        return;
    }

    mCurrentBatch->m_updateMeshArray.add(mesh);
    mCurrentBatch->m_dirtyFlag |= RenderBatchData::MeshUpdateDirty;
}

bool RenderBackendService::endRenderBatch() {
    if (nullptr == mCurrentBatch) {
        return false;
    }

    if (nullptr == mCurrentPass) {
        mCurrentPass = new PassData("defaultPass", nullptr);
    }

    if (-1 == hasBatch(mCurrentBatch->m_id, mCurrentPass->mMeshBatches)) {
        mCurrentPass->mMeshBatches.add(mCurrentBatch);
    }

    mCurrentBatch = nullptr;

    return true;
}

bool RenderBackendService::endPass() {
    if (nullptr == mCurrentPass) {
        return false;
    }

    if (-1 == hasPass(mCurrentPass->m_id, mPasses)) {
        mPasses.add(mCurrentPass);
    }
    mCurrentPass = nullptr;

    return true;
}

void RenderBackendService::clearPasses() {
    mCurrentPass = nullptr;

    for (ui32 i = 0; i < mPasses.size(); ++i) {
        delete mPasses[i];
    }
    mPasses.clear();
    mFrameCreated = false;
}

void RenderBackendService::attachView() {    
    // empty
}

void RenderBackendService::resize(ui32 w, ui32 h) {
    if (mBehaviour.ResizeViewport) {
        setViewport(0, 0, w, h);
        ResizeEventData *data = new ResizeEventData(0, 0, w, h);
        mRenderTaskPtr->sendEvent(&OnResizeEvent, data);
    }
}

void RenderBackendService::focusLost() {
    // todo: implement me
}

void RenderBackendService::syncRenderThread() {
    // Synchronizing event with render back-end
    auto result = mRenderTaskPtr->sendEvent(&OnRenderFrameEvent, nullptr);
    if(!result) {
        osre_debug(Tag, "Error while requesting next frame.");
    }
    mRenderTaskPtr->awaitUpdate();
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
