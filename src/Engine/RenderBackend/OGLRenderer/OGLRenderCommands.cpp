/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "OGLRenderCommands.h"

#include "OGLCommon.h"
#include "OGLRenderBackend.h"
#include "OGLRenderEventHandler.h"
#include "OGLShader.h"
#include "RenderCmdBuffer.h"

#include <osre/App/AssetRegistry.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Platform/AbstractOGLRenderContext.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/THWBufferManager.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Shader.h>

namespace OSRE {
namespace RenderBackend {

static const c8 *Tag = "OGLRenderCommands";

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

bool setupTextures(Material *mat, OGLRenderBackend *rb, TArray<OGLTexture *> &textures) {
    OSRE_ASSERT(nullptr != mat);
    OSRE_ASSERT(nullptr != rb);

    if (nullptr == mat) {
        osre_debug(Tag, "Material is nullptr.");
        return false;
    }

    if (nullptr == rb) {
        osre_debug(Tag, "Renderbackend is nullptr.");
        return false;
    }

    const size_t numTextures(mat->m_numTextures);
    if (0 == numTextures) {
        return true;
    }

    for (ui32 i = 0; i < numTextures; ++i) {
        Texture *tex(mat->m_textures[i]);
        if (!tex->m_textureName.empty()) {
            OGLTexture *oglTexture = rb->createTexture(tex->m_textureName, tex);
            if (nullptr != oglTexture) {
                textures.add(oglTexture);
            }
        }
    }

    return true;
}

SetMaterialStageCmdData *setupMaterial(Material *material, OGLRenderBackend *rb, OGLRenderEventHandler *eh) {
    OSRE_ASSERT(nullptr != eh);
    OSRE_ASSERT(nullptr != material);
    OSRE_ASSERT(nullptr != rb);

    if (nullptr == material || nullptr == rb || nullptr == eh) {
        return nullptr;
    }
    SetMaterialStageCmdData *matData = new SetMaterialStageCmdData;
    switch (material->m_type) {
        case MaterialType::ShaderMaterial: {
            TArray<OGLTexture *> textures;
            setupTextures(material, rb, textures);
            OGLRenderCmd *renderMatCmd = new OGLRenderCmd(OGLRenderCmdType::SetMaterialCmd);
            if (!textures.isEmpty()) {
                matData->m_textures = textures;
            }

            OGLShader *shader = rb->createShader("mat", material->m_shader);
            if (nullptr != shader) {
                matData->m_shader = shader;
                for (ui32 i = 0; i < material->m_shader->m_attributes.size(); i++) {
                    const String &attribute = material->m_shader->m_attributes[i];
                    shader->addAttribute(attribute);
                }

                for (ui32 i = 0; i < material->m_shader->m_parameters.size(); i++) {
                    shader->addUniform(material->m_shader->m_parameters[i]);
                }

                // for setting up all buffer objects
                eh->setActiveShader(shader);
            }
            renderMatCmd->m_data = matData;
            eh->enqueueRenderCmd(renderMatCmd);
        } break;
        default:
            break;
    }

    return matData;
}

void setupParameter(UniformVar *param, OGLRenderBackend *rb, OGLRenderEventHandler *ev) {
    OSRE_ASSERT(nullptr != param);
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != ev);

    if (nullptr == param || nullptr == rb || nullptr == ev) {
        return;
    }

    ::CPPCore::TArray<OGLParameter *> paramArray;
    OGLParameter *oglParam = rb->getParameter(param->m_name);
    if (nullptr == oglParam) {
        oglParam = rb->createParameter(param->m_name, param->m_type, &param->m_data, param->m_numItems);
    } else {
        ::memcpy(oglParam->m_data->getData(), param->m_data.getData(), param->m_data.m_size);
    }

    paramArray.add(oglParam);
    ev->setParameter(paramArray);
}

OGLVertexArray *setupBuffers(Mesh *mesh, OGLRenderBackend *rb, OGLShader *oglShader) {
    OSRE_ASSERT(nullptr != mesh);
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != oglShader);

    if (nullptr == mesh || nullptr == rb || nullptr == oglShader) {
        return nullptr;
    }

    rb->useShader(oglShader);

    OGLVertexArray *vertexArray = rb->createVertexArray();
    rb->bindVertexArray(vertexArray);
    BufferData *vertices = mesh->m_vb;
    if (nullptr == vertices) {
        osre_debug(Tag, "No vertex buffer data for setting up data.");
        return nullptr;
    }

    BufferData *indices = mesh->m_ib;
    if (nullptr == indices) {
        osre_debug(Tag, "No index buffer data for setting up data.");
        return nullptr;
    }

    // create vertex buffer and  and pass triangle vertex to buffer object
    OGLBuffer *vb = rb->createBuffer(vertices->m_type);
    vb->m_geoId = mesh->m_id;
    rb->bindBuffer(vb);
    rb->copyDataToBuffer(vb, vertices->getData(), vertices->getSize(), vertices->m_access);

    // enable vertex attribute arrays
    TArray<OGLVertexAttribute *> attributes;
    rb->createVertexCompArray(mesh->m_vertextype, oglShader, attributes);
    const ui32 stride = Mesh::getVertexSize(mesh->m_vertextype);
    rb->bindVertexLayout(vertexArray, oglShader, stride, attributes);
    rb->releaseVertexCompArray(attributes);

    // create index buffer and pass indices to element array buffer
    OGLBuffer *ib = rb->createBuffer(indices->m_type);
    ib->m_geoId = mesh->m_id;
    rb->bindBuffer(ib);
    rb->copyDataToBuffer(ib, indices->getData(), indices->getSize(), indices->m_access);

    rb->unbindVertexArray();

    return vertexArray;
}

void setupPrimDrawCmd(const char *id, bool useLocalMatrix, const glm::mat4 &model,
        const TArray<size_t> &primGroups, OGLRenderBackend *rb,
        OGLRenderEventHandler *eh, OGLVertexArray *va) {
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != eh);

    if (primGroups.isEmpty()) {
        return;
    }

    OGLRenderCmd *renderCmd = new OGLRenderCmd(OGLRenderCmdType::DrawPrimitivesCmd);
    DrawPrimitivesCmdData *data = new DrawPrimitivesCmdData;
    if (useLocalMatrix) {
        data->m_model = model;
        data->m_localMatrix = useLocalMatrix;
    }
    data->m_id = id;
    data->m_vertexArray = va;
    data->m_primitives.reserve(primGroups.size());
    for (ui32 i = 0; i < primGroups.size(); ++i) {
        data->m_primitives.add(primGroups[i]);
    }
    renderCmd->m_data = static_cast<void *>(data);

    eh->enqueueRenderCmd(renderCmd);
}

void setupInstancedDrawCmd(const char *id, const TArray<size_t> &ids, OGLRenderBackend *rb,
        OGLRenderEventHandler *eh, OGLVertexArray *va, size_t numInstances) {
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != eh);

    if (ids.isEmpty()) {
        return;
    }

    OGLRenderCmd *renderCmd = new OGLRenderCmd(OGLRenderCmdType::DrawPrimitivesInstancesCmd);

    DrawInstancePrimitivesCmdData *data = new DrawInstancePrimitivesCmdData;
    data->m_id = id;
    data->m_vertexArray = va;
    data->m_numInstances = numInstances;
    data->m_primitives.reserve(ids.size());
    for (ui32 j = 0; j < ids.size(); ++j) {
        data->m_primitives.add(ids[j]);
    }
    renderCmd->m_data = static_cast<void *>(data);
    eh->enqueueRenderCmd(renderCmd);
}

} // Namespace RenderBackend
} // Namespace OSRE
