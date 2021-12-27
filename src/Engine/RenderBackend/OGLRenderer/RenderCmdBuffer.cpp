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

RenderCmdBuffer::RenderCmdBuffer(OGLRenderBackend *renderBackend, AbstractOGLRenderContext *ctx, Pipeline *pipeline) :
        m_renderbackend(renderBackend),
        m_renderCtx(ctx),
        m_activeShader(nullptr),
        m_2dShader(nullptr),
        m_primitives(),
        m_materials(),
        m_paramArray(),
        m_matrixBuffer(),
        m_pipeline(pipeline) {
    osre_assert(nullptr != m_renderbackend);
    osre_assert(nullptr != m_renderCtx);
    osre_assert(nullptr != m_pipeline);

    m_clearState.m_state = (i32)ClearState::ClearBitType::ColorBit | (i32)ClearState::ClearBitType::DepthBit;
}

RenderCmdBuffer::~RenderCmdBuffer() {
    clear();

    m_renderbackend = nullptr;
    m_renderCtx = nullptr;
}

void RenderCmdBuffer::setActiveShader(OGLShader *oglShader) {
    m_activeShader = oglShader;
}

OGLShader *RenderCmdBuffer::getActiveShader() const {
    return m_activeShader;
}

void RenderCmdBuffer::enqueueRenderCmd(OGLRenderCmd *renderCmd, EnqueueType type) {
    if (nullptr == renderCmd) {
        osre_debug(Tag, "Nullptr to render-command detected.");
        return;
    }
    if (nullptr == renderCmd->m_data) {
        osre_debug(Tag, "Nullptr in render-command data detected.");
        return;
    }

    if (EnqueueType::PushBack == type) {
        m_cmdbuffer.add(renderCmd);
    }
}

void RenderCmdBuffer::enqueueRenderCmdGroup(const String &groupName, CPPCore::TArray<OGLRenderCmd *> &cmdGroup, EnqueueType type) {
    if (groupName.empty()) {
        osre_debug(Tag, "No name for render command group defined.");
        return;
    }

    if (cmdGroup.isEmpty()) {
        osre_debug(Tag, "Render command group is empty.");
        return;
    }

    if (EnqueueType::PushBack == type) {
        m_cmdbuffer.add(&cmdGroup[0], cmdGroup.size());
    }
}

void RenderCmdBuffer::onPreRenderFrame() {
    osre_assert(nullptr != m_renderbackend);

    if (nullptr == m_renderCtx) {
        return;
    }

    m_renderCtx->activate();
    m_renderbackend->clearRenderTarget(m_clearState);
}

void RenderCmdBuffer::onRenderFrame(const EventData *) {
    osre_assert(nullptr != m_renderbackend);

    size_t numPasses = m_pipeline->beginFrame();
    if (0 == numPasses) {
        return;
    }

    for (ui32 passId = 0; passId < numPasses; passId++) {
        RenderPass *pass = m_pipeline->beginPass(passId);
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
        m_renderbackend->setFixedPipelineStates(states);

        for (OGLRenderCmd *renderCmd : m_cmdbuffer) {
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

        m_pipeline->endPass(passId);
    }
    m_pipeline->endFrame();

    m_renderbackend->renderFrame();
}

void RenderCmdBuffer::onPostRenderFrame() {
    osre_assert(nullptr != m_renderbackend);

    // unbind the active shader
    m_renderbackend->useShader(nullptr);
    m_renderbackend->unbindVertexArray();
}

void RenderCmdBuffer::clear() {
    ContainerClear(m_cmdbuffer);
    m_paramArray.resize(0);
}

static bool hasParam(const String &name, const ::CPPCore::TArray<OGLParameter *> &paramArray) {
    for (ui32 i = 0; i < paramArray.size(); i++) {
        if (name == paramArray[i]->m_name) {
            return true;
        }
    }
    return false;
}

void RenderCmdBuffer::setParameter(OGLParameter *param) {
    if (!hasParam(param->m_name, m_paramArray)) {
        m_paramArray.add(param);
    }
}

void RenderCmdBuffer::setParameter(const ::CPPCore::TArray<OGLParameter *> &paramArray) {
    for (ui32 i = 0; i < paramArray.size(); i++) {
        if (!hasParam(paramArray[i]->m_name, m_paramArray)) {
            m_paramArray.add(paramArray[i]);
        }
    }
}

void RenderCmdBuffer::commitParameters() {
    m_renderbackend->setMatrix(MatrixType::Model, m_model);
    m_renderbackend->setMatrix(MatrixType::View, m_view);
    m_renderbackend->setMatrix(MatrixType::Projection, m_proj);
    m_renderbackend->applyMatrix();

    for (ui32 i = 0; i < m_paramArray.size(); i++) {
        m_renderbackend->setParameter(m_paramArray[i]);
    }
}

void RenderCmdBuffer::setMatrixes(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj) {
    m_model = model;
    m_view = view;
    m_proj = proj;
}

void RenderCmdBuffer::setMatrixBuffer(const c8 *id, MatrixBuffer *buffer) {
    osre_assert(nullptr != id);

    m_matrixBuffer[id] = buffer;
}

bool RenderCmdBuffer::onDrawPrimitivesCmd(DrawPrimitivesCmdData *data) {
    osre_assert(nullptr != m_renderbackend);
    if (nullptr == data) {
        return false;
    }

    std::map<const char *, MatrixBuffer *>::iterator it = m_matrixBuffer.find(data->m_id);
    if (it != m_matrixBuffer.end()) {
        MatrixBuffer *buffer = it->second;
        setMatrixes(buffer->m_model, buffer->m_view, buffer->m_proj);
    }

    m_renderbackend->bindVertexArray(data->m_vertexArray);
    if (data->m_localMatrix) {
        m_renderbackend->setMatrix(MatrixType::Model, data->m_model);
        m_renderbackend->applyMatrix();
    }
    for (size_t i = 0; i < data->m_primitives.size(); ++i) {
        m_renderbackend->render(data->m_primitives[i]);
    }

    return true;
}

bool RenderCmdBuffer::onDrawPrimitivesInstancesCmd(DrawInstancePrimitivesCmdData *data) {
    osre_assert(nullptr != m_renderbackend);
    if (nullptr == data) {
        return false;
    }

    m_renderbackend->bindVertexArray(data->m_vertexArray);
    for (size_t i = 0; i < data->m_primitives.size(); i++) {
        m_renderbackend->render(data->m_primitives[i], data->m_numInstances);
    }

    return true;
}

const String shader_2d_vs = 
		"#version 410\n"
        "in vec2 vp;"
        "uniform mat4 V, P;"
        "out vec2 st;"
        "void main () {"
        "  st = (vp + 1.0) * 0.5;"
        "  gl_Position = P * V * vec4 (10.0 * vp.x, -1.0, 10.0 * -vp.y, 1.0);"
        "}";

const String shader_2d_fs =
        "in vec2 st;"
        "uniform sampler2D tex;"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = texture (tex, st);"
        "}";

        
bool RenderCmdBuffer::onDrawPanelCmd(DrawPanelsCmdData *data) {
    osre_assert(nullptr != m_renderbackend);
    if (nullptr == data) {
        return false;
    }
    if (nullptr == m_2dShader) {
        Shader shader_2d;
        shader_2d.setSource(ShaderType::SH_VertexShaderType, shader_2d_vs);
        shader_2d.setSource(ShaderType::SH_FragmentShaderType, shader_2d_fs);
        m_2dShader = m_renderbackend->createShader("2d", &shader_2d);
    }
    m_renderbackend->useShader(m_2dShader); 
    for (size_t i = 0; i < data->mNumPanels; ++i) {
    }

    return true;
}

bool RenderCmdBuffer::onSetRenderTargetCmd(SetRenderTargetCmdData *) {
    osre_assert(nullptr != m_renderbackend);

    return true;
}

bool RenderCmdBuffer::onSetMaterialStageCmd(SetMaterialStageCmdData *data) {
    osre_assert(nullptr != m_renderbackend);

    m_renderbackend->bindVertexArray(data->m_vertexArray);
    m_renderbackend->useShader(data->m_shader);

    commitParameters();

    for (ui32 i = 0; i < data->m_textures.size(); ++i) {
        OGLTexture *oglTexture = data->m_textures[i];
        if (nullptr != oglTexture) {
            m_renderbackend->bindTexture(oglTexture, (TextureStageType)i);
        } else {
            m_renderbackend->unbindTexture((TextureStageType)i);
        }
    }

    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
