#pragma once

#include <osre/Common/osre_common.h>

#include <cppcore/Container/TArray.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace RenderBackend {

class OGLRenderContext;
class OGLRenderBackend;
class OGLShader;
class RenderCmdBuffer;
class HWBufferManager;
class OGLRenderEventHandler;
class Mesh;

struct Vertex;
struct OGLVertexArray;
struct OGLTexture;
struct PrimitiveGroup;
struct Material;
struct OGLRenderCmd;
struct DrawPrimitivesCmdData;
struct SetTextureStageCmdData;
struct SetShaderStageCmdData;
struct SetRenderTargetCmdData;
struct OGLParameter;
struct UniformVar;
struct SetMaterialStageCmdData;

bool setupTextures(Material* mat, OGLRenderBackend* rb, CPPCore::TArray<OGLTexture*>& textures);
SetMaterialStageCmdData* setupMaterial(Material* material, OGLRenderBackend* rb, OGLRenderEventHandler* eh);
void setupParameter(UniformVar* param, OGLRenderBackend* rb, OGLRenderEventHandler* ev);
OGLVertexArray* setupBuffers(Mesh* mesh, OGLRenderBackend* rb, OGLShader* oglShader);
void setupPrimDrawCmd(const char* id, bool useLocalMatrix, const glm::mat4& model,
    const CPPCore::TArray<size_t>& primGroups, OGLRenderBackend* rb,
    OGLRenderEventHandler* eh, OGLVertexArray* va);
void setupInstancedDrawCmd(const char* id, const CPPCore::TArray<size_t>& ids, OGLRenderBackend* rb,
    OGLRenderEventHandler* eh, OGLVertexArray* va, size_t numInstances);

} // Namespace RenderBackend
} // Namespace OSRE
