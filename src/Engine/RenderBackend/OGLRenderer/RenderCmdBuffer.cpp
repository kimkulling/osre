/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/Shader.h>
#include "Engine/RenderBackend/OGLRenderer/RenderCmdBuffer.h"
#include "OGLCommon.h"
#include "OGLRenderBackend.h"
#include <osre/Debugging/osre_debugging.h>
#include <osre/Platform/AbstractOGLRenderContext.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

static const c8 *Tag = "RenderCmdBuffer";

RenderCmdBuffer::RenderCmdBuffer(OGLRenderBackend *renderBackend, AbstractOGLRenderContext *ctx) :
        mRBService(renderBackend),
        mRenderCtx(ctx),
        mActiveShader(nullptr),
        mPrimitives(),
        mMaterials(),
        mParamArray(),
        mMatrixBuffer(),
        mPipeline(nullptr) {
    osre_assert(nullptr != mRBService);
    osre_assert(nullptr != mRenderCtx);

    mClearState.m_state = (i32)ClearState::ClearBitType::ColorBit | (i32)ClearState::ClearBitType::DepthBit;
}

RenderCmdBuffer::~RenderCmdBuffer() {
    clear();

    mRBService = nullptr;
    mRenderCtx = nullptr;
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
    if (0 == numPasses) {
        return;
    }

    for (ui32 passId = 0; passId < numPasses; passId++) {
        RenderPass *pass = mPipeline->beginPass(passId);
        if (nullptr == pass) {
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

static bool hasParam(const String &name, const ::cppcore::TArray<OGLParameter *> &paramArray) {
    for (ui32 i = 0; i < paramArray.size(); i++) {
        if (name == paramArray[i]->m_name) {
            return true;
        }
    }
    return false;
}

void RenderCmdBuffer::setParameter(OGLParameter *param) {
    if (hasParam(param->m_name, mParamArray)) {
        return;
    }

    mParamArray.add(param);
}

void RenderCmdBuffer::setParameter(const ::cppcore::TArray<OGLParameter *> &paramArray) {
    for (ui32 i = 0; i < paramArray.size(); i++) {
        if (!hasParam(paramArray[i]->m_name, mParamArray)) {
            mParamArray.add(paramArray[i]);
        }
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
}

void RenderCmdBuffer::setMatrixBuffer(const c8 *id, MatrixBuffer *buffer) {
    osre_assert(nullptr != id);

    mMatrixBuffer[id] = buffer;
}

bool RenderCmdBuffer::onDrawPrimitivesCmd(DrawPrimitivesCmdData *data) {
    if (nullptr == data) {
        return false;
    }

    std::map<const char *, MatrixBuffer *>::iterator it = mMatrixBuffer.find(data->m_id);
    if (it != mMatrixBuffer.end()) {
        MatrixBuffer *buffer = it->second;
        setMatrixes(buffer->m_model, buffer->m_view, buffer->m_proj);
    }

    mRBService->bindVertexArray(data->m_vertexArray);
    if (data->m_localMatrix) {
        glm::mat4 model = mRBService->getMatrix(MatrixType::Model);
        mRBService->setMatrix(MatrixType::Model, data->m_model*model);
        mRBService->applyMatrix();
    }
    for (size_t i = 0; i < data->m_primitives.size(); ++i) {
        mRBService->render(data->m_primitives[i]);
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

} // Namespace RenderBackend
} // Namespace OSRE
