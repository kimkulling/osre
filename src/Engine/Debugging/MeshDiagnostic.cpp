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
#include <osre/Debugging/MeshDiagnostic.h>
#include <osre/Common/Logger.h>

#include <iostream>

namespace OSRE {
namespace Debugging {

using namespace ::OSRE::RenderBackend;

static const c8* Tag = "MeshDiagnostic";

void MeshDiagnostic::dumpTextBox(ui32 i, glm::vec3* textPos, ui32 VertexOffset) {
    std::stringstream stream;
    stream << std::endl;
    stream << "i = " << i << " : " << textPos[VertexOffset + 0].x << ", " << textPos[VertexOffset + 0].y << ", " << textPos[VertexOffset + 0].z << "\n";
    stream << "i = " << i << " : " << textPos[VertexOffset + 1].x << ", " << textPos[VertexOffset + 1].y << ", " << textPos[VertexOffset + 1].z << "\n";
    stream << "i = " << i << " : " << textPos[VertexOffset + 2].x << ", " << textPos[VertexOffset + 2].y << ", " << textPos[VertexOffset + 2].z << "\n";
    stream << "i = " << i << " : " << textPos[VertexOffset + 3].x << ", " << textPos[VertexOffset + 3].y << ", " << textPos[VertexOffset + 3].z << "\n";
    osre_info(Tag, stream.str());
}

void MeshDiagnostic::dumpTextTex0Box(ui32 i, glm::vec2* tex0Pos, ui32 VertexOffset) {
    std::stringstream stream;
    stream << std::endl;
    stream << "i = " << i << " : " << tex0Pos[VertexOffset + 0].x << ", " << tex0Pos[VertexOffset + 0].y << "\n";
    stream << "i = " << i << " : " << tex0Pos[VertexOffset + 1].x << ", " << tex0Pos[VertexOffset + 1].y << "\n";
    stream << "i = " << i << " : " << tex0Pos[VertexOffset + 2].x << ", " << tex0Pos[VertexOffset + 2].y << "\n";
    stream << "i = " << i << " : " << tex0Pos[VertexOffset + 3].x << ", " << tex0Pos[VertexOffset + 3].y << "\n";
    osre_info(Tag, stream.str());
}

static void dumpRenderVertex(ui32 idx, const RenderBackend::RenderVert& vertex) {
    std::cout << "v[" << idx << "].position = " << vertex.position.x << "|" << vertex.position.y << "|" << vertex.position.z << "\n";
    std::cout << "v[" << idx << "].normal   = " << vertex.normal.x << "|" << vertex.normal.y << "|" << vertex.normal.z << "\n";
    std::cout << "v[" << idx << "].color0   = " << vertex.color0.x << "|" << vertex.color0.y << "|" << vertex.color0.z << "\n";
    std::cout << "v[" << idx << "].tex0     = " << vertex.tex0.x << "|" << vertex.tex0.y << "\n";
}

void MeshDiagnostic::dumpVertices(RenderBackend::RenderVert* renderVertices, ui32 numVertices) {
    if (0 == numVertices || nullptr == renderVertices) {
        return;
    }

    for (ui32 i = 0; i < numVertices; i++) {
        dumpRenderVertex(i, renderVertices[i]);
    }
}

void MeshDiagnostic::dumpUiVertexCache(const UiVertexCache& vertexCache) {
    if (vertexCache.isEmpty()) {
        return;
    }

    std::stringstream stream;
    stream << std::endl;
    for (size_t idx = 0; idx < vertexCache.numVertices(); ++idx) {
        const RenderVert& vertex = vertexCache.m_cache[idx];
        stream << "v[" << idx << "].position = " << vertex.position.x << "|" << vertex.position.y << "|" << vertex.position.z << "\n";
        stream << "v[" << idx << "].normal   = " << vertex.normal.x << "|" << vertex.normal.y << "|" << vertex.normal.z << "\n";
        stream << "v[" << idx << "].color0   = " << vertex.color0.x << "|" << vertex.color0.y << "|" << vertex.color0.z << "\n";
        stream << "v[" << idx << "].tex0     = " << vertex.tex0.x << "|" << vertex.tex0.y << "\n";
    }
    stream << "\n";
    osre_info(Tag, stream.str());
}

void MeshDiagnostic::dumpVertices(const CPPCore::TArray<RenderBackend::RenderVert>& renderVertices) {
    if (renderVertices.isEmpty()) {
        return;
    }
    for (ui32 i = 0; i < renderVertices.size(); i++) {
        dumpRenderVertex(i, renderVertices[i]);
    }
}

void MeshDiagnostic::dumpIndices(const CPPCore::TArray<ui16>& indexArray) {
    if (indexArray.isEmpty()) {
        return;
    }
    std::stringstream stream;
    stream << std::endl;

    for (ui32 i = 0; i < indexArray.size(); i++) {
        std::cout << indexArray[i] << ", ";
    }
    std::cout << "\n";
}

void MeshDiagnostic::dumpIndices(ui16* indices, ui32 numIndices) {
    if (nullptr == indices || 0 == numIndices) {
        return;
    }

    std::stringstream stream;
    stream << std::endl;
    for (ui32 i = 0; i < numIndices; i++) {
        stream << indices[i] << ", ";
    }
    stream << "\n";
    osre_info(Tag, stream.str());
}

void MeshDiagnostic::dumpUiIndexCache(const UiIndexCache& indexCache) {
    if (indexCache.isEmpty() ) {
        return;
    }

    std::stringstream stream;
    stream << std::endl;
    for (size_t i = 0; i < indexCache.numIndices(); ++i){
        stream << indexCache.m_cache[i] << ", ";
    }
    stream << "\n";
    osre_info(Tag, stream.str());
}

void MeshDiagnostic::dumpIndices(const CPPCore::TArray<ui32>& indexArray) {
    if (indexArray.isEmpty()) {
        return;
    }

    for (ui32 i = 0; i < indexArray.size(); i++) {
        std::cout << indexArray[i] << ", ";
    }
    std::cout << "\n";

}

}
}
