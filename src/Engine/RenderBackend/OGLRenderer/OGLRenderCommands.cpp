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
#include "OGLRenderCommands.h"

#include "OGLCommon.h"
#include "RenderBackend/Material.h"
#include "OGLRenderBackend.h"
#include "OGLRenderEventHandler.h"
#include "OGLShader.h"
#include "RenderCmdBuffer.h"

#include "Common/Logger.h"
#include "Debugging/osre_debugging.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/Shader.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace OSRE::RenderBackend {

DECL_OSRE_LOG_MODULE(OGLRenderCommands)

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::cppcore;

bool makeScreenShot(const c8 *filename, ui32 w, ui32 h) {
    const size_t numberOfPixels = w * h * 3;
    TArray<uc8> pixels;
    pixels.resize(numberOfPixels);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, &pixels[0]);
    bool result = true;
    if (stbi_write_jpg(filename, w, h, 3, &pixels[0], static_cast<i32>(numberOfPixels)) != 0){
        result = false;
    }

    return result;
}

bool setupTextures(Material *mat, OGLRenderBackend *rb, TArray<OGLTexture *> &textures) {
    if (nullptr == mat) {
        osre_debug(Tag, "Material is nullptr.");
        return false;
    }

    if (nullptr == rb) {
        osre_debug(Tag, "Renderbackend is nullptr.");
        return false;
    }

    const size_t numTextures = mat->getNumTextures();
    if (numTextures == 0) {
        return true;
    }

    for (ui32 i = 0; i < numTextures; ++i) {
        Texture *tex = mat->getTextureStageAt(i);
        if (tex == nullptr) {
            osre_error(Tag, "Texture instance is nullptr.");
            continue;
        }

        if (!tex->TextureName.empty()) {
            OGLTexture *oglTexture = rb->createTexture(tex->TextureName, tex);
            if (nullptr != oglTexture) {
                textures.add(oglTexture);
            } else {
                textures.add(rb->createDefaultTexture(tex->TargetType, tex->PixelFormat, tex->Width, tex->Height));
            }
        }
    }

    return true;
}

SetMaterialStageCmdData *setupMaterial(Material *material, OGLRenderBackend *rb, OGLRenderEventHandler *eh) {
    if (nullptr == material || nullptr == rb || nullptr == eh) {
        return nullptr;
    }
    
    auto *matData = new SetMaterialStageCmdData;
    switch (material->getMaterialType()) {
        case MaterialType::ShaderMaterial: {
            TArray<OGLTexture *> textures;
            setupTextures(material, rb, textures);
            auto *renderMatCmd = new OGLRenderCmd(OGLRenderCmdType::SetMaterialCmd);
            if (!textures.isEmpty()) {
                matData->textures = textures;
            }
            const String name = material->getShader()->getName();
            Shader *matShader = material->getShader();
            OGLShader *shader = rb->createShader(name, matShader);
            if (nullptr != shader) {
                matData->shader = shader;
                for (size_t i = 0; i < matShader->getNumVertexAttributes(); ++i) {
                    shader->addAttribute(matShader->getVertexAttributeAt(i));
                }

                for (size_t i = 0; i < matShader->getNumUniformBuffer(); ++i) {
                    shader->addUniform(matShader->getUniformBufferAt(i));
                }

                // for setting up all buffer objects
                eh->setActiveShader(shader);
            }
            renderMatCmd->data = matData;
            eh->enqueueRenderCmd(renderMatCmd);
        }                                  
        break;
    default:
        osre_debug(Tag, "Unsupported material type.");
        break;
    }

    return matData;
}

void setupParameter(UniformVar *param, OGLRenderBackend *rb, OGLRenderEventHandler *ev) {
    if (nullptr == param || nullptr == rb || nullptr == ev) {
        return;
    }

    TArray<OGLParameter *> paramArray;
    OGLParameter *oglParam = rb->getParameter(param->m_name);
    if (nullptr == oglParam) {
        oglParam = rb->createParameter(param->m_name, param->m_type, &param->m_data, param->m_numItems);
    } else {
        memcpy(oglParam->data->getData(), param->m_data.getData(), param->m_data.m_size);
    }

    paramArray.add(oglParam);
    ev->setParameter(paramArray);
}

OGLVertexArray *setupBuffers(Mesh *mesh, OGLRenderBackend *rb, OGLShader *oglShader) {
    if (mesh == nullptr || rb == nullptr || oglShader == nullptr) {
        return nullptr;
    }

    rb->useShader(oglShader);

    OGLVertexArray *vertexArray = rb->createVertexArray();
    rb->bindVertexArray(vertexArray);
    BufferData *vertices = mesh->getVertexBuffer();
    if (vertices == nullptr) {
        osre_error(Tag, "No vertex buffer data for setting up data.");
        return nullptr;
    }

    BufferData *indices = mesh->getIndexBuffer();
    if (indices == nullptr) {
        osre_error(Tag, "No index buffer data for setting up data.");
        return nullptr;
    }

    // create vertex buffer and  and pass triangle vertex to buffer object
    OGLBuffer *vb = rb->createBuffer(vertices->m_type);
    vb->geoId = mesh->getId();
    rb->bindBuffer(vb);
    rb->copyDataToBuffer(vb, vertices->getData(), vertices->getSize(), vertices->m_access);

    // enable vertex attribute arrays
    TArray<OGLVertexAttribute *> attributes;
    rb->createVertexCompArray(mesh->getVertexType(), oglShader, attributes);
    const size_t stride = Mesh::getVertexSize(mesh->getVertexType());
    rb->bindVertexLayout(vertexArray, oglShader, stride, attributes);
    rb->releaseVertexCompArray(attributes);

    // create index buffer and pass indices to element array buffer
    OGLBuffer *ib = rb->createBuffer(indices->m_type);
    ib->geoId = mesh->getId();
    rb->bindBuffer(ib);
    rb->copyDataToBuffer(ib, indices->getData(), indices->getSize(), indices->m_access);

    rb->unbindVertexArray();

    return vertexArray;
}

void setupPrimDrawCmd(const char *id, bool useLocalMatrix, const glm::mat4 &model,
        const TArray<size_t> &primGroups, OGLRenderBackend *rb,
        OGLRenderEventHandler *eh, OGLVertexArray *va) {
    if (id == nullptr || rb == nullptr || eh == nullptr || va == nullptr) {
        osre_error(Tag, "Invalid parameter.");
        return;
    }
    if (primGroups.isEmpty()) {
        osre_error(Tag, "Primitive group is empty.");
        return;
    }

    auto *renderCmd = new OGLRenderCmd(OGLRenderCmdType::DrawPrimitivesCmd);
    auto *drawPrimitiveCmdData = new DrawPrimitivesCmdData;
    if (useLocalMatrix) {
        drawPrimitiveCmdData->model = model;
        drawPrimitiveCmdData->localMatrix = useLocalMatrix;
    }
    drawPrimitiveCmdData->id = id;
    drawPrimitiveCmdData->vertexArray = va;
    drawPrimitiveCmdData->primitives.reserve(primGroups.size());
    for (ui32 i = 0; i < primGroups.size(); ++i) {
        drawPrimitiveCmdData->primitives.add(primGroups[i]);
    }
    renderCmd->data = static_cast<void*>(drawPrimitiveCmdData);

    eh->enqueueRenderCmd(renderCmd);
}

void setupInstancedDrawCmd(const char *id, const TArray<size_t> &ids, OGLRenderBackend *rb,
        OGLRenderEventHandler *eh, OGLVertexArray *va, size_t numInstances) {
    osre_assert(nullptr != rb);
    osre_assert(nullptr != eh);

    if (ids.isEmpty()) {
        return;
    }

    OGLRenderCmd *renderCmd = new OGLRenderCmd(OGLRenderCmdType::DrawPrimitivesInstancesCmd);

    DrawInstancePrimitivesCmdData *data = new DrawInstancePrimitivesCmdData;
    data->id = id;
    data->vertexArray = va;
    data->numInstances = numInstances;
    data->primitives.reserve(ids.size());
    for (ui32 j = 0; j < ids.size(); ++j) {
        data->primitives.add(ids[j]);
    }
    renderCmd->data = static_cast<void *>(data);
    eh->enqueueRenderCmd(renderCmd);
}

} // Namespace OSRE::RenderBackend
