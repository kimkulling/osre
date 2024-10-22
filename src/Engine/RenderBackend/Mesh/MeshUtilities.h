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
#ifndef OSRE_RENDERBACKEND_MESHUTILITIES_H_INC
#define OSRE_RENDERBACKEND_MESHUTILITIES_H_INC

#include "Common/osre_common.h"
#include "Debugging/osre_debugging.h"
#include "Common/Tokenizer.h"

namespace OSRE {
namespace RenderBackend {

///-----------------------------------------------------------------
/// @class MeshUtilities
///
/// @brief This class provides common utilities to generate different
///        kind of mesh topology and spatial data.
///-----------------------------------------------------------------
class MeshUtilities {
public:
    ///	@brief Named constants for the number of vertices and indices for a quad.
    static constexpr size_t NumQuadVert = 4;
    static constexpr ui32 NumQuadIndices = 6;

    /// @brief Will calculate the numbr of vertices needed for the given string glyphes.
    /// @param text     The text for checking.
    /// @return The number of vertives for the glyphes.
    static size_t getNumTextVerts( const String &text ) {
        const size_t NumTextVerts = NumQuadVert * text.size();
        return NumTextVerts;
    }

    /// @brief Will calculate the number of indices needed for the given text glyphes.
    /// @param text     The text for checking.
    /// @return The number of indices
    static size_t getNumTextIndices(const String &text) {
        const size_t numIndices = NumQuadIndices * text.size();
        return numIndices;
    }

    /// @brief Will generate the vertices and indices for the text box and all glyphes in it.
    /// @param x            The upper left coordinate component in x.
    /// @param y            The upper left coordinate component in y.
    /// @param textSize     The requested text size.
    /// @param text         The text itself to render.
    /// @param positions    The positios for all glyphes.
    /// @param colors       The vertex colors for all glyphes.
    /// @param tex0         The diffuse texture coordinates for all glyphes.
    /// @param textIndices  The index array for all glyphes.
    static void generateTextBoxVerticesAndIndices(f32 x, f32 y, f32 textSize, const String &text,
             Vec3Array &positions, Vec3Array &colors, Vec2Array &tex0, ui16 **textIndices) {
        osre_assert(nullptr != textIndices);

        using namespace ::OSRE::Common;
        glm::vec3 col[NumQuadVert] = {};
        col[0] = glm::vec3(0, 0, 0);
        col[1] = glm::vec3(0, 0, 0);
        col[2] = glm::vec3(0, 0, 0);
        col[3] = glm::vec3(0, 0, 0);

        glm::vec3 pos[NumQuadVert] = {};
        pos[0] = glm::vec3(x, y, 0);
        pos[1] = glm::vec3(x, y + textSize, 0);
        pos[2] = glm::vec3(x + textSize, y, 0);
        pos[3] = glm::vec3(x + textSize, y + textSize, 0);

        const size_t NumTextVerts = getNumTextVerts(text);
        positions.resize(NumTextVerts);
        colors.resize(NumTextVerts);
        tex0.resize(NumTextVerts);
        *textIndices = new ui16[getNumTextIndices(text)];

        const f32 invCol = 1.f / 16.f;
        const f32 invRow = 1.f / 16.f;
        ui32 textCol(0), textRow(0);
        for (ui32 i = 0; i < text.size(); i++) {
            const c8 ch = text[i];
            if (Tokenizer::isLineBreak(ch)) {
                textCol = 0;
                ++textRow;
                continue;
            }

            const ui16 VertexOffset(static_cast<ui16>(i) * static_cast<ui16>(NumQuadVert));
            const f32  rowHeight(-1.0f * textRow * textSize);
            positions[VertexOffset + 0].x = pos[0].x + (textCol * textSize);
            positions[VertexOffset + 0].y = pos[0].y + rowHeight;
            positions[VertexOffset + 0].z = 0;

            positions[VertexOffset + 1].x = pos[1].x + (textCol * textSize);
            positions[VertexOffset + 1].y = pos[1].y + rowHeight;
            positions[VertexOffset + 1].z = 0;

            positions[VertexOffset + 2].x = pos[2].x + (textCol * textSize);
            positions[VertexOffset + 2].y = pos[2].y + rowHeight;
            positions[VertexOffset + 2].z = 0;

            positions[VertexOffset + 3].x = pos[3].x + (textCol * textSize);
            positions[VertexOffset + 3].y = pos[3].y + rowHeight;
            positions[VertexOffset + 3].z = 0;

            //GeometryDiagnosticUtils::dumpTextBox( i, textPos, VertexOffset );

            const i32 column = (ch) % 16;
            const i32 row = (ch) / 16;
            const f32 s = column * invCol;
            const f32 t = (row + 1) * invRow;

            tex0[VertexOffset + 0].x = s;
            tex0[VertexOffset + 0].y = 1.0f - t;

            tex0[VertexOffset + 1].x = s;
            tex0[VertexOffset + 1].y = 1.0f - t + 1.0f / 16.0f;

            tex0[VertexOffset + 2].x = s + 1.0f / 16.0f;
            tex0[VertexOffset + 2].y = 1.0f - t;

            tex0[VertexOffset + 3].x = s + 1.0f / 16.0f;
            tex0[VertexOffset + 3].y = 1.0f - t + 1.0f / 16.0f;

            //GeometryDiagnosticUtils::dumpTextTex0Box(i, tex0, VertexOffset);
            colors[VertexOffset + 0] = col[0];
            colors[VertexOffset + 1] = col[1];
            colors[VertexOffset + 2] = col[2];
            colors[VertexOffset + 3] = col[3];
            const ui32 IndexOffset(i * NumQuadIndices);
            (*textIndices)[0 + IndexOffset] = 0 + VertexOffset;
            (*textIndices)[1 + IndexOffset] = 2 + VertexOffset;
            (*textIndices)[2 + IndexOffset] = 1 + VertexOffset;

            (*textIndices)[3 + IndexOffset] = 1 + VertexOffset;
            (*textIndices)[4 + IndexOffset] = 2 + VertexOffset;
            (*textIndices)[5 + IndexOffset] = 3 + VertexOffset;
            ++textCol;
        }
    }
};

} // namespace RenderBackend
} // namespace OSRE

#endif // OSRE_RENDERBACKEND_MESHUTILITIES_H_INC
