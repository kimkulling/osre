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
#include "RenderBackend/OGLRenderer/RenderCmdBuffer.h"
#include "RenderBackend/OGLRenderer/OGLCommon.h"
#include "RenderBackend/OGLRenderer/OGLRenderBackend.h"
#include "Debugging/osre_debugging.h"
#include "Platform/AbstractOGLRenderContext.h"

namespace OSRE::RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

DECL_OSRE_LOG_MODULE(RenderCmdBuffer)

RenderCmdBuffer::RenderCmdBuffer(OGLRenderBackend *renderBackend, AbstractOGLRenderContext *ctx) :
        mRBService(renderBackend),
        mRenderCtx(ctx),
        mActiveShader(nullptr),
        mPipeline(nullptr) {
    mClearState.m_state = (i32)ClearState::ClearBitType::ColorBit | (i32)ClearState::ClearBitType::DepthBit;
}

RenderCmdBuffer::~RenderCmdBuffer() {
    clear();
}

void RenderCmdBuffer::setActiveShader(OGLShader *oglShader) {
    mActiveShader = oglShader;
}

OGLShader *RenderCmdBuffer::getActiveShader() const {
    return mActiveShader;
}

void RenderCmdBuffer::enqueueRenderCmd(OGLRenderCmd *renderCmd) {
    if (nullptr == renderCmd) {
        osre_debug(Tag, "Nullptr to render-command detected.");
        return;
    }

    if (nullptr == renderCmd->m_data) {
        osre_debug(Tag, "Nullptr in render-command data detected.");
        return;
    }

    mCommandQueue.add(renderCmd);
}

void RenderCmdBuffer::enqueueRenderCmdGroup(const String &groupName, cppcore::TArray<OGLRenderCmd *> &cmdGroup) {
    if (groupName.empty()) {
        osre_debug(Tag, "No name for render command group defined.");
        return;
    }

    if (cmdGroup.isEmpty()) {
        osre_debug(Tag, "Render command group is empty.");
        return;
    }

    mCommandQueue.add(&cmdGroup[0], cmdGroup.size());
}

void RenderCmdBuffer::onPreRenderFrame(Pipeline *pipeline) {
    if (pipeline == nullptr) {
        return;
    }
    mPipeline = pipeline;
    mRenderCtx->activate();
    mRBService->clearRenderTarget(mClearState);
}

void RenderCmdBuffer::onRenderFrame() {
    if (mPipeline == nullptr) {
        return;
    }

    const size_t numPasses = mPipeline->beginFrame();
    if (numPasses == 0) {
        return;
    }

    for (ui32 passId = 0; passId < numPasses; ++passId) {
        RenderPass *pass = mPipeline->beginPass(passId);
        if (pass == nullptr) {
            osre_debug(Tag, "Ponter to pipeline pass is nullptr.");
            continue;
        }

        RenderStates states;
        states.m_polygonState = pass->getPolygonState();
        states.m_cullState = pass->getCullState();
        states.m_blendState = pass->getBlendState();
        states.m_samplerState = pass->getSamplerState();
        states.m_stencilState = pass->getStencilState();
        mRBService->setFixedPipelineStates(states);
        mRBService->setMatrix(MatrixType::View, pass->getView());
        mRBService->setMatrix(MatrixType::Projection, pass->getProjection());
        const Viewport &v = pass->getViewport();
        mRBService->setViewport(v.m_x, v.m_y, v.m_w, v.m_h);
        for (OGLRenderCmd *renderCmd : mCommandQueue) {
            if (nullptr == renderCmd) {
                continue;
            }

            if (renderCmd->m_type == OGLRenderCmdType::DrawPrimitivesCmd) {
                onDrawPrimitivesCmd((DrawPrimitivesCmdData *)renderCmd->m_data);
            } else if (renderCmd->m_type == OGLRenderCmdType::DrawPrimitivesInstancesCmd) {
                onDrawPrimitivesInstancesCmd((DrawInstancePrimitivesCmdData *)renderCmd->m_data);
            } else if (renderCmd->m_type == OGLRenderCmdType::SetRenderTargetCmd) {
                onSetRenderTargetCmd((SetRenderTargetCmdData *)renderCmd->m_data);
            } else if (renderCmd->m_type == OGLRenderCmdType::SetMaterialCmd) {
                onSetMaterialStageCmd((SetMaterialStageCmdData *)renderCmd->m_data);
            } else {
                osre_error(Tag, "Unsupported render command type: " + static_cast<ui32>(renderCmd->m_type));
            }
        }

        mPipeline->endPass(passId);
    }
    mPipeline->endFrame();

    mRBService->renderFrame();
}

void RenderCmdBuffer::onPostRenderFrame() {

    // unbind the active shader
    mRBService->useShader(nullptr);
    mRBService->unbindVertexArray();

    mPipeline = nullptr;
}

void RenderCmdBuffer::clear() {
    ContainerClear(mCommandQueue);
    mParamArray.resize(0);
}

static bool hasParam(const String &name, const ::cppcore::TArray<OGLParameter *> &paramArray, size_t &index) {
    index = paramArray.size();
    for (ui32 i = 0; i < paramArray.size(); i++) {
        if (name == paramArray[i]->m_name) {
            index = i;
            return true;
        }
    }
    return false;
}

void RenderCmdBuffer::setParameter(OGLParameter *param) {
    size_t i = 0;
    if (hasParam(param->m_name, mParamArray, i)) {
        mParamArray[i] = param;
    }

    mParamArray.add(param);
}

void RenderCmdBuffer::setParameter(const ::cppcore::TArray<OGLParameter *> &paramArray) {
    for (ui32 i = 0; i < paramArray.size(); i++) {
        size_t index = 0;
        OGLParameter *param = paramArray[i];
        osre_assert(param != nullptr);
        if (!hasParam(paramArray[i]->m_name, mParamArray, index)) {
            mParamArray.add(param);
            return;
        }
        mParamArray[index] = param;
    }
}

void RenderCmdBuffer::commitParameters() {
    mRBService->setMatrix(MatrixType::Model, mModel);
    mRBService->setMatrix(MatrixType::View, mView);
    mRBService->setMatrix(MatrixType::Projection, mProj);
    mRBService->applyMatrix();

    for (ui32 i = 0; i < mParamArray.size(); i++) {
        mRBService->setParameter(mParamArray[i]);
    }
}

void RenderCmdBuffer::setMatrixes(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj) {
    mModel = model;
    mView = view;
    mProj = proj;
    commitParameters();
}

void RenderCmdBuffer::setMatrixBuffer(const c8 *id, MatrixBuffer *buffer) {
    assert(nullptr != id);

    mMatrixBuffer[id] = buffer;
}

bool RenderCmdBuffer::onDrawPrimitivesCmd(DrawPrimitivesCmdData *data) {
    if (nullptr == data) {
        return false;
    }

    if (data->id == nullptr) {
        return false;
    }

    if (auto it = mMatrixBuffer.find(data->id); it != mMatrixBuffer.end()) {
        const MatrixBuffer *buffer = it->second;
        setMatrixes(buffer->model, buffer->view, buffer->proj);
    }

    mRBService->bindVertexArray(data->vertexArray);
    if (data->localMatrix) {
        glm::mat4 model = mRBService->getMatrix(MatrixType::Model);
        mRBService->setMatrix(MatrixType::Model, data->model * model);
        mRBService->applyMatrix();
    }

    for (size_t i = 0; i < data->primitives.size(); ++i) {
        mRBService->render(data->primitives[i]);
    }

    return true;
}

bool RenderCmdBuffer::onDrawPrimitivesInstancesCmd(DrawInstancePrimitivesCmdData *data) {
    if (nullptr == data) {
        return false;
    }

    mRBService->bindVertexArray(data->m_vertexArray);
    for (size_t i = 0; i < data->m_primitives.size(); i++) {
        mRBService->render(data->m_primitives[i], data->m_numInstances);
    }

    return true;
}

bool RenderCmdBuffer::onSetRenderTargetCmd(SetRenderTargetCmdData *data) {
    if (data->mFrameBuffer == nullptr) {
        return true;
    }

    mRBService->bindFrameBuffer(data->mFrameBuffer);
    mRBService->clearRenderTarget(data->mClearState);

    return true;
}

bool RenderCmdBuffer::onSetMaterialStageCmd(SetMaterialStageCmdData *data) {
    mRBService->bindVertexArray(data->m_vertexArray);
    mRBService->useShader(data->m_shader);

    commitParameters();

    for (ui32 i = 0; i < data->m_textures.size(); ++i) {
        OGLTexture *oglTexture = data->m_textures[i];
        if (nullptr != oglTexture) {
            mRBService->bindTexture(oglTexture, (TextureStageType)i);
        } else {
            mRBService->unbindTexture((TextureStageType)i);
        }
    }

    return true;
}

} // namespace OSRE::RenderBackend