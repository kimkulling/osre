/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "RenderBackend/RenderBackendService.h"
#include "Profiling/PerformanceCounterRegistry.h"
#include "Properties/Settings.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/DbgRenderer.h"
#include "Threading/SystemTask.h"
#include "Debugging/MeshDiagnostic.h"
#include "OGLRenderer/OGLRenderEventHandler.h"
#ifdef OSRE_WINDOWS
#   include "Platform/Windows/MinWindows.h"
#endif

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
        if (0 == strncmp(passDataArray[i]->id, id, strlen(id))) {
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
        if (0 == strncmp(batchDataArray[i]->id, id, strlen(id))) {
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
    data->NextFrame = mSubmitFrame;

    mRenderTaskPtr->sendEvent(&OnInitPassesEvent, data);
}

void RenderBackendService::commitNextFrame() {
    if (!mRenderTaskPtr.isValid()) {
        return;
    }

    CommitFrameEventData *data = CommitFrameEventData::create();
    data->NextFrame = mSubmitFrame;
    for (ui32 i = 0; i < mPasses.size(); ++i) {
        PassData *currentPass = mPasses[i];
        if (currentPass == nullptr) {
            continue;
        }

        for (ui32 j = 0; j < currentPass->meshBatches.size(); ++j) {
            RenderBatchData *currentBatch = currentPass->meshBatches[j];
            if (currentBatch == nullptr) {
                continue;
            }

            if (currentBatch->needsUpdate(RenderBatchData::MatrixBufferUpdate)) {
                FrameSubmitCmd *cmd = mSubmitFrame->enqueue(currentPass->id, currentBatch->id);
                currentBatch->matrixBuffer.m_view = currentPass->view;
                currentBatch->matrixBuffer.m_proj = currentPass->proj;
                assert(cmd->batchId != nullptr);
                cmd->updateFlags |= (ui32) FrameSubmitCmd::UpdateMatrixes;
                cmd->size = sizeof(MatrixBuffer);
                cmd->data = new c8[cmd->size];

                ::memcpy(cmd->data, &currentBatch->matrixBuffer, cmd->size);
            }

            if (currentBatch->needsUpdate(RenderBatchData::UniformBufferUpdate)) {
                UniformBuffer &uniformBuffer = data->NextFrame->uniforBuffers[i];
                for (ui32 k = 0; k < currentBatch->uniforms.size(); ++k) {
                    FrameSubmitCmd *cmd = mSubmitFrame->enqueue(currentPass->id, currentBatch->id);
                    assert(cmd->batchId != nullptr);
                    cmd->updateFlags |= (ui32)FrameSubmitCmd::UpdateUniforms;
                    UniformVar *var = currentBatch->uniforms[k];
                    if (nullptr == var) {
                        continue;
                    }

                    uniformBuffer.writeVar(var);

                    // todo: replace by uniform buffer.
                    cmd->size = var->size();
                    cmd->data = new c8[cmd->size];
                    size_t offset = 0;
                    cmd->data[offset] = var->name.size() > 255 ? 255 : static_cast<c8>(var->name.size());
                    ++offset;
                    ::memcpy(&cmd->data[offset], var->name.c_str(), var->name.size());
                    offset += var->name.size();
                    ::memcpy(&cmd->data[offset], var->data.getData(), var->data.size);
                }
            }

            if (currentBatch->needsUpdate(RenderBatchData::MeshUpdateUpdate)) {
                for (ui32 k = 0; k < currentBatch->updateMeshArray.size(); ++k) {
                    FrameSubmitCmd *cmd = mSubmitFrame->enqueue(currentPass->id, currentBatch->id);
                    cmd->updateFlags |= (ui32)FrameSubmitCmd::UpdateBuffer;
                    Mesh *currentMesh = currentBatch->updateMeshArray[k];
                    cmd->meshId = currentMesh->getId();
                    cmd->size = currentMesh->getVertexBuffer()->getSize();
                    cmd->data = new c8[cmd->size];
                    ::memcpy(cmd->data, currentMesh->getVertexBuffer()->getData(), cmd->size);
                }
            }

            if (currentBatch->needsUpdate(RenderBatchData::MeshUpdate)) {
                FrameSubmitCmd *cmd = mSubmitFrame->enqueue(currentPass->id, currentBatch->id);
                PassData *pd = new PassData(currentPass->id, nullptr);
                pd->meshBatches.add(currentBatch);
                cmd->updatedPasses.add(pd);
                cmd->updateFlags |= (ui32)FrameSubmitCmd::AddRenderData;
            }

            currentBatch->clearFlags();
        }
    }

    data->NextFrame = mSubmitFrame;
    std::swap(mSubmitFrame, mRenderFrame);
    osre_assert(mSubmitFrame != mRenderFrame);

    mRenderTaskPtr->sendEvent(&OnCommitFrameEvent, data);
}

void RenderBackendService::sendEvent(const Event *ev, const EventData *eventData) {
    osre_assert(ev != nullptr);

    if (mRenderTaskPtr.isValid()) {
        mRenderTaskPtr->sendEvent(ev, eventData);
    }
}

Pipeline *RenderBackendService::createDefault3DPipeline() {
    Pipeline *pipeline = new Pipeline(DefaultPipelines::get3DPipelineDefault());
    RenderPass *renderPass = nullptr;
    renderPass = RenderPassFactory::create(RenderPassId);
    CullState cullState(CullState::CullMode::CCW, CullState::CullFace::Back);
    renderPass->setCullState(cullState);
    renderPass->setViewport(mViewport);
    pipeline->addPass(renderPass);

    return pipeline;
}

Pipeline* RenderBackendService::createDefault2DPipeline() {
    Pipeline *pipeline = new Pipeline(DefaultPipelines::get2DPipelineDefault());
    RenderPass *renderPass = nullptr;
    renderPass = RenderPassFactory::create(UiPassId);
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
        if (0 == ::strncmp(mCurrentPass->id, id, strlen(id))) {
            return mCurrentPass;
        }
    }

    for (ui32 i = 0; i < mPasses.size(); ++i) {
        if (0 == ::strncmp(mPasses[i]->id, id, strlen(id))) {
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
        mCurrentPass->viewport = mViewport;
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

    mCurrentPass->renderTarget = fb;
}

void RenderBackendService::setMatrix(MatrixType type, const glm::mat4 &m) {
    if (nullptr == mCurrentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    switch (type) {
        case MatrixType::Model:
            mCurrentBatch->matrixBuffer.m_model = m;
            mCurrentBatch->dirtyFlag |= RenderBatchData::MatrixBufferUpdate;
            break;
        case MatrixType::View:
            mCurrentPass->view = m;
            mCurrentBatch->matrixBuffer.m_view = m;
            mCurrentBatch->dirtyFlag |= RenderBatchData::MatrixBufferUpdate;
            break;
        case MatrixType::Projection:
            mCurrentPass->proj = m;
            mCurrentBatch->matrixBuffer.m_proj = m;
            mCurrentBatch->dirtyFlag |= RenderBatchData::MatrixBufferUpdate;
            break;
        default:
            osre_warn(Tag, "Unknown matrix type.");
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
        mCurrentBatch->uniforms.add(var);
    }

    mCurrentBatch->dirtyFlag |= RenderBatchData::UniformBufferUpdate;
    ::memcpy(var->data.data, glm::value_ptr(matrix), sizeof(glm::mat4));
}

void RenderBackendService::addUniform(UniformVar *uniformVar) {
    if (nullptr == uniformVar) {
        osre_error(Tag, "Invalid uniform.");
        return;
    }

    if (nullptr == mCurrentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    mCurrentBatch->uniforms.add(uniformVar);
    mCurrentBatch->dirtyFlag |= RenderBatchData::UniformBufferUpdate;
}

void RenderBackendService::setMatrixArray(const String &name, ui32 numMat, const glm::mat4 *matrixArray) {
    if (nullptr == mCurrentBatch) {
        osre_error(Tag, "No active batch.");
        return;
    }

    UniformVar *var = mCurrentBatch->getVarByName(name.c_str());
    if (nullptr == var) {
        var = UniformVar::create(name, ParameterType::PT_Mat4Array, numMat);
        mCurrentBatch->uniforms.add(var);
    }

    ::memcpy(var->data.data, glm::value_ptr(matrixArray[0]), sizeof(glm::mat4) * numMat);
    mCurrentBatch->dirtyFlag |= RenderBatchData::UniformBufferUpdate;
}

void RenderBackendService::addMesh(Mesh *mesh, ui32 numInstances) {
    if (mesh == nullptr) {
        osre_error(Tag, "Pointer to geometry is nullptr.");
        return;
    }

    if (mCurrentBatch == nullptr) {
        osre_error(Tag, "No active batch.");
        return;
    }

    MeshEntry *entry = new MeshEntry;
    entry->mMeshArray.add(mesh);
    entry->numInstances = numInstances;
    mCurrentBatch->meshArray.add(entry);
    mCurrentBatch->dirtyFlag |= RenderBatchData::MeshUpdate;
}

void RenderBackendService::addMesh(const MeshArray &meshArray, ui32 numInstances) {
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
    mCurrentBatch->meshArray.add(entry);
    mCurrentBatch->dirtyFlag |= RenderBatchData::MeshUpdate;
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

    mCurrentBatch->updateMeshArray.add(mesh);
    mCurrentBatch->dirtyFlag |= RenderBatchData::MeshUpdateUpdate;
}

bool RenderBackendService::endRenderBatch() {
    if (nullptr == mCurrentBatch) {
        return false;
    }

    if (nullptr == mCurrentPass) {
        mCurrentPass = new PassData("defaultPass", nullptr);
    }

    if (-1 == hasBatch(mCurrentBatch->id, mCurrentPass->meshBatches)) {
        mCurrentPass->meshBatches.add(mCurrentBatch);
    }

    mCurrentBatch = nullptr;

    return true;
}

bool RenderBackendService::endPass() {
    if (nullptr == mCurrentPass) {
        return false;
    }

    if (-1 == hasPass(mCurrentPass->id, mPasses)) {
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
    mViewport.x = x;
    mViewport.y = y;
    mViewport.w = w;
    mViewport.h = h;
}

const Viewport &RenderBackendService::getViewport() const {
    return mViewport;
}

} // Namespace RenderBackend
} // Namespace OSRE
