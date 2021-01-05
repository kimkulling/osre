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
#pragma once

#include <osre/Common/osre_common.h>
#include <osre/Math/BaseMath.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

class OGLRenderContext;
class OGLRenderBackend;
class OGLShader;
class RenderCmdBuffer;
//class HWBufferManager;
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
