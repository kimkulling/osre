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
#include "OGLRenderCommands.h"

#include "OGLCommon.h"
#include <osre/RenderBackend/Material.h>
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
#include <osre/Platform/AbstractTimer.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Shader.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace OSRE {
namespace RenderBackend {

static constexpr c8 Tag[] = "OGLRenderCommands";

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::cppcore;

bool makeScreenShot(const c8 *filename, ui32 w, ui32 h) {
    const i32 numberOfPixels = w * h * 3;
    unsigned char *pixels = new uc8[numberOfPixels];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, w, h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    bool result = true;
    if (stbi_write_jpg(filename, w, h, 3, pixels, numberOfPixels) != 0){
        result = false;
    }

    delete [] pixels;

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
            } else {
                textures.add(rb->createDefaultTexture(tex->m_targetType, tex->mPixelFormat, tex->m_width, tex->m_height));
            }
        }
    }

    return true;
}

SetMaterialStageCmdData *setupMaterial(Material *material, OGLRenderBackend *rb, OGLRenderEventHandler *eh) {
    osre_assert(nullptr != eh);
    osre_assert(nullptr != material);
    osre_assert(nullptr != rb);

    if (nullptr == material || nullptr == rb || nullptr == eh) {
        return nullptr;
    }
    
    auto *matData = new SetMaterialStageCmdData;
    switch (material->m_type) {
        case MaterialType::ShaderMaterial: {
            TArray<OGLTexture *> textures;
            setupTextures(material, rb, textures);
            auto *renderMatCmd = new OGLRenderCmd(OGLRenderCmdType::SetMaterialCmd);
            if (!textures.isEmpty()) {
                matData->m_textures = textures;
            }
            String name = "mat";
            name += material->m_name;
            OGLShader *shader = rb->createShader(name, material->m_shader);
            if (nullptr != shader) {
                matData->m_shader = shader;
                for (size_t i = 0; i < material->m_shader->getNumVertexAttributes(); ++i) {
                    shader->addAttribute(material->m_shader->getVertexAttributeAt(i));
                }

                for (size_t i = 0; i < material->m_shader->getNumUniformBuffer(); ++i) {
                    shader->addUniform(material->m_shader->getUniformBufferAt(i));
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
    osre_assert(nullptr != param);
    osre_assert(nullptr != rb);
    osre_assert(nullptr != ev);

    if (nullptr == param || nullptr == rb || nullptr == ev) {
        return;
    }

    ::cppcore::TArray<OGLParameter *> paramArray;
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
    if (mesh == nullptr || rb == nullptr || oglShader == nullptr) {
        return nullptr;
    }

    rb->useShader(oglShader);

    OGLVertexArray *vertexArray = rb->createVertexArray();
    rb->bindVertexArray(vertexArray);
    BufferData *vertices = mesh->getVertexBuffer();
    if (vertices == nullptr) {
        osre_debug(Tag, "No vertex buffer data for setting up data.");
        return nullptr;
    }

    BufferData *indices = mesh->getIndexBuffer();
    if (indices == nullptr) {
        osre_debug(Tag, "No index buffer data for setting up data.");
        return nullptr;
    }

    // create vertex buffer and  and pass triangle vertex to buffer object
    OGLBuffer *vb = rb->createBuffer(vertices->m_type);
    vb->m_geoId = mesh->getId();
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
    ib->m_geoId = mesh->getId();
    rb->bindBuffer(ib);
    rb->copyDataToBuffer(ib, indices->getData(), indices->getSize(), indices->m_access);

    rb->unbindVertexArray();

    return vertexArray;
}

void setupPrimDrawCmd(const char *id, bool useLocalMatrix, const glm::mat4 &model,
        const TArray<size_t> &primGroups, OGLRenderBackend *rb,
        OGLRenderEventHandler *eh, OGLVertexArray *va) {
    if (rb == nullptr || eh == nullptr || va == nullptr) {
        return;
    }
    if (primGroups.isEmpty()) {
        return;
    }

    auto *renderCmd = new OGLRenderCmd(OGLRenderCmdType::DrawPrimitivesCmd);
    auto *drawPrimitiveCmdData = new DrawPrimitivesCmdData;
    if (useLocalMatrix) {
        drawPrimitiveCmdData->m_model = model;
        drawPrimitiveCmdData->m_localMatrix = useLocalMatrix;
    }
    drawPrimitiveCmdData->m_id = id;
    drawPrimitiveCmdData->m_vertexArray = va;
    drawPrimitiveCmdData->m_primitives.reserve(primGroups.size());
    for (ui32 i = 0; i < primGroups.size(); ++i) {
        drawPrimitiveCmdData->m_primitives.add(primGroups[i]);
    }
    renderCmd->m_data = static_cast<void*>(drawPrimitiveCmdData);

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
