#include "OGLRenderCommands.h"

#include "OGLRenderBackend.h"
#include "OGLShader.h"
#include "OGLCommon.h"
#include "OGLRenderEventHandler.h"
#include "RenderCmdBuffer.h"

#include <osre/Common/Logger.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/AbstractOGLRenderContext.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Shader.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/HWBufferManager.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/App/AssetRegistry.h>


namespace OSRE {
namespace RenderBackend {

static const c8* Tag = "OGLRenderCommands";

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

bool setupTextures(Material* mat, OGLRenderBackend* rb, TArray<OGLTexture*>& textures) {
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
        Texture* tex(mat->m_textures[i]);
        if (!tex->m_textureName.empty()) {
            /*String root = Assets::AssetRegistry::getPath("media");
            String path = Assets::AssetRegistry::resolvePathFromUri(tex->m_loc);

            IO::Uri loc(tex->m_loc);
            loc.setPath(path);*/

            OGLTexture* oglTexture = rb->createTexture(tex->m_textureName, tex);
            //OGLTexture* oglTexture = rb->createTextureFromFile(tex->m_textureName, loc);
            if (nullptr != oglTexture) {
                textures.add(oglTexture);
            }
        }
    }

    return true;
}

SetMaterialStageCmdData* setupMaterial(Material* material, OGLRenderBackend* rb, OGLRenderEventHandler* eh) {
    OSRE_ASSERT(nullptr != eh);
    OSRE_ASSERT(nullptr != material);
    OSRE_ASSERT(nullptr != rb);

    SetMaterialStageCmdData* matData = new SetMaterialStageCmdData;
    switch (material->m_type) {
        case MaterialType::ShaderMaterial: {
            TArray<OGLTexture*> textures;
            setupTextures(material, rb, textures);
            OGLRenderCmd* renderMatCmd = OGLRenderCmdAllocator::alloc(OGLRenderCmdType::SetMaterialCmd, nullptr);
            if (!textures.isEmpty()) {
                matData->m_textures = textures;
            }

            OGLShader* shader = rb->createShader("mat", material->m_shader);
            if (nullptr != shader) {
                matData->m_shader = shader;
                for (ui32 i = 0; i < material->m_shader->m_attributes.size(); i++) {
                    const String& attribute = material->m_shader->m_attributes[i];
                    //if ( shader->hasAttribute( attribute ) ) {
                    shader->addAttribute(attribute);
                    //}
                }

                for (ui32 i = 0; i < material->m_shader->m_parameters.size(); i++) {
                    shader->addUniform(material->m_shader->m_parameters[i]);
                }

                // for setting up all buffer objects
                eh->setActiveShader(shader);
            }
            renderMatCmd->m_data = matData;
            eh->enqueueRenderCmd(renderMatCmd);
        }
                                         break;

        default:
            break;
    }

    return matData;
}

void setupParameter(UniformVar* param, OGLRenderBackend* rb, OGLRenderEventHandler* ev) {
    OSRE_ASSERT(nullptr != param);
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != ev);

    if (!param) {
        return;
    }

    ::CPPCore::TArray<OGLParameter*> paramArray;
    OGLParameter* oglParam = rb->getParameter(param->m_name);
    if (nullptr == oglParam) {
        oglParam = rb->createParameter(param->m_name, param->m_type, &param->m_data, param->m_numItems);
    }
    else {
        ::memcpy(oglParam->m_data->getData(), param->m_data.getData(), param->m_data.m_size);
    }

    paramArray.add(oglParam);
    ev->setParameter(paramArray);
}

OGLVertexArray* setupBuffers(Mesh* mesh, OGLRenderBackend* rb, OGLShader* oglShader) {
    OSRE_ASSERT(nullptr != mesh);
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != oglShader);

    rb->useShader(oglShader);

    OGLVertexArray* vertexArray = rb->createVertexArray();
    rb->bindVertexArray(vertexArray);
    BufferData* vertices = mesh->m_vb;
    if (nullptr == vertices) {
        osre_debug(Tag, "No vertex buffer data for setting up data.");
        return nullptr;
    }

    BufferData* indices = mesh->m_ib;
    if (nullptr == indices) {
        osre_debug(Tag, "No index buffer data for setting up data.");
        return nullptr;
    }

    // create vertex buffer and  and pass triangle vertex to buffer object
    OGLBuffer* vb = rb->createBuffer(vertices->m_type);
    vb->m_geoId = mesh->m_id;
    rb->bindBuffer(vb);
    rb->copyDataToBuffer(vb, vertices->getData(), vertices->getSize(), vertices->m_access);

    // enable vertex attribute arrays
    TArray<OGLVertexAttribute*> attributes;
    rb->createVertexCompArray(mesh->m_vertextype, oglShader, attributes);
    const ui32 stride = Mesh::getVertexSize(mesh->m_vertextype);
    rb->bindVertexLayout(vertexArray, oglShader, stride, attributes);
    rb->releaseVertexCompArray(attributes);

    // create index buffer and pass indices to element array buffer
    OGLBuffer* ib = rb->createBuffer(indices->m_type);
    ib->m_geoId = mesh->m_id;
    rb->bindBuffer(ib);
    rb->copyDataToBuffer(ib, indices->getData(), indices->getSize(), indices->m_access);

    rb->unbindVertexArray();

    return vertexArray;
}

void setupPrimDrawCmd(const char* id, bool useLocalMatrix, const glm::mat4& model,
        const TArray<size_t>& primGroups, OGLRenderBackend* rb,
        OGLRenderEventHandler* eh, OGLVertexArray* va) {
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != eh);

    if (primGroups.isEmpty()) {
        return;
    }

    OGLRenderCmd* renderCmd = OGLRenderCmdAllocator::alloc(OGLRenderCmdType::DrawPrimitivesCmd, nullptr);
    DrawPrimitivesCmdData* data = new DrawPrimitivesCmdData;
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
    renderCmd->m_data = static_cast<void*>(data);

    eh->enqueueRenderCmd(renderCmd);
}

void setupInstancedDrawCmd(const char* id, const TArray<size_t>& ids, OGLRenderBackend* rb,
        OGLRenderEventHandler* eh, OGLVertexArray* va, size_t numInstances) {
    OSRE_ASSERT(nullptr != rb);
    OSRE_ASSERT(nullptr != eh);

    if (ids.isEmpty()) {
        return;
    }

    OGLRenderCmd* renderCmd = OGLRenderCmdAllocator::alloc(OGLRenderCmdType::DrawPrimitivesInstancesCmd, nullptr);

    DrawInstancePrimitivesCmdData* data = new DrawInstancePrimitivesCmdData;
    data->m_id = id;
    data->m_vertexArray = va;
    data->m_numInstances = numInstances;
    data->m_primitives.reserve(ids.size());
    for (ui32 j = 0; j < ids.size(); ++j) {
        data->m_primitives.add(ids[j]);
    }
    renderCmd->m_data = static_cast<void*>(data);
    eh->enqueueRenderCmd(renderCmd);
}

} // Namespace RenderBackend
} // Namespace OSRE

