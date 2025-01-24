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
#pragma once

#include "Common/osre_common.h"
#include "RenderBackend/RenderCommon.h"

namespace OSRE {
namespace Debugging {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This utility class is used to inspect mesh data.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT MeshDiagnostic {
public:
    /// @brief  Will dump the vertex data of a text-box.
    /// @param[in] i            The start index.
    /// @param[in] textPos      The text position array.
    /// @param[in] vertexOffset The start offset for the vertices.
    static void dumpTextBox(ui32 i, glm::vec3* textPos, ui32 vertexOffset);
    
    /// @brief  Will dump the vertex-texture-coordinates data of a text-box.
    /// @param[in] i            THe start index.
    /// @param[in] tex0Pos      The array containing the text positions.
    /// @param[in] vertexOffset The vertex offset for the dump.
    static void dumpTextTex0Box(ui32 i, glm::vec2* tex0Pos, ui32 vertexOffset);
    
    /// @brief  Will dump the vertex data of given vertices.
    /// @param[in] renderVertices   The render vertices to dump.
    /// @param[in] numVerices       The number of vertices to dump.
    static void dumpVertices(RenderBackend::RenderVert* renderVertices, ui32 numVerices);
    
    /// @brief Will dump the vertex ui cache.
    /// @param[in] vertexCache  The vertexcache to dump.
    static void dumpUiVertexCache(const RenderBackend::UiVertexCache& vertexCache);

    /// @brief  Will dump the vertex data of given vertices.
    /// @param[in] renderVertices The render vertices container to dump.
    static void dumpVertices(const cppcore::TArray<RenderBackend::RenderVert>& renderVertices);

    /// @brief  Will dump the index data of given indices.
    /// @param[in] indexArray   The index array to dump.
    static void dumpIndices(const cppcore::TArray<ui16>& indexArray);
    
    /// @brief  Will dump the index data of given indices.
    /// @param[in] indices      The index array to dump.
    /// @param[in] numINdices   The number of indices to dump.
    static void dumpIndices(ui16* indices, ui32 numIndices);
    
    /// @brief  Will dump an index cache
    /// @param[in] indexCache   The index cache to dump.
    static void dumpUiIndexCache(const RenderBackend::UiIndexCache &indexCache);
    
    /// @brief  Will dump the index data of given indices.
    /// @param[in] indexArray   The index array to dump.
    static void dumpIndices(const cppcore::TArray<ui32>& indexArray);
    
    /// @brief Will dump a 4x4 matrix.
    /// @param[in] mat          The matrix to dump,
    static void dump_matrix(const glm::mat4 &mat);

    MeshDiagnostic() = delete;
    MeshDiagnostic(const MeshDiagnostic&) = delete;
    MeshDiagnostic& operator = (const MeshDiagnostic&) = delete;
};

}
} // namespace OSRE
