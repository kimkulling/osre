/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/RenderCommon.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Forward declarations
namespace OSRE {

namespace RenderBackend {
    struct Geometry;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This utility class is used to inspect geometry data.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT GeometryDiagnosticUtils {
public:
	static void dumpTextBox( ui32 i, glm::vec3 *textPos, ui32 VertexOffset );
	static void dumpTextTex0Box( ui32 i, glm::vec2 *tex0Pos, ui32 VertexOffset );
    static void dumVertices( RenderBackend::RenderVert *renderVertices, ui32 numverices );
    static void dumVertices( const CPPCore::TArray<RenderBackend::RenderVert> &renderVertices );
	static void dumpIndices( const CPPCore::TArray<ui16> &indexArray );
    static void dumpIndices( ui16 *indices, ui32 numIndices );
    static void dumpIndices( const CPPCore::TArray<ui32> &indexArray );
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to create the static geometry for base shapes like a triangle or a 
///	text box. 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT GeometryBuilder {
public:
    /// @brief  The class constructor.
    GeometryBuilder();

    /// @brief  The class destructor.
    ~GeometryBuilder();

    /// @brief  Will allocate an empty geometry.
    ///	@param  type        [in] The vertex type.
    /// @return The created geometry.
    static RenderBackend::Geometry *allocEmptyGeometry( RenderBackend::VertexType type, ui32 numGeo );

    /// @brief  Will allocate a triangle geometry.
    ///	@param  type        [in] The vertex type.
    /// @return The created geometry.
    static RenderBackend::Geometry *allocTriangles( RenderBackend::VertexType type, RenderBackend::BufferAccessType access );

    ///	@brief  Will allocate vertices for a quad primitive.
    ///	@param  type        [in] The vertex type.
    /// @return The created geometry.
    static RenderBackend::Geometry *allocQuads( RenderBackend::VertexType type, RenderBackend::BufferAccessType access );

    static RenderBackend::Geometry *allocCube( RenderBackend::VertexType type, RenderBackend::BufferAccessType access );

    ///	@brief  Will allocate vertices for a list of lines.
    ///	@param  type        [in] The vertex type.
    static RenderBackend::Geometry *allocLineList( RenderBackend::VertexType type, RenderBackend::BufferAccessType access,
            ui32 numLines, glm::vec3 *posArray, glm::vec3 *colorArray, ui32 *indices );

    static RenderBackend::Geometry *allocPoints( RenderBackend::VertexType type, RenderBackend::BufferAccessType access, 
            ui32 numPoints, glm::vec3 *posArray, glm::vec3 *colorArray );

    ///	@brief  Will allocate vertices for a text-box.
    /// @param  x           [in] Left position of the text box.
    /// @param  y           [in] Upper position of the text box.
    /// @param  textSize    [in] The size for a single glyph.
    /// @param  text        [in] The text to render.
    /// @return The created geometry.
    static RenderBackend::Geometry *allocTextBox( f32 x, f32 y, f32 textSize, const String &text, RenderBackend::BufferAccessType access );

    /// @brief  Will update the vertices of a text box.
    /// @param  geo         [inout] The geometry to update.
    /// @param  textSize    [in] The new text size.
    /// @param  text        [in] The updated text.
    static void updateTextBox( RenderBackend::Geometry *geo, f32 textSize, const String &text );

    ///	@brief  Allocates vertices into a buffer data.
    /// @param  type        [in] The vertex type to create.
    ///	@param  numVerts    [in] The number of vertices to create.
    ///	@param  pos         [in] Pointer to array with vec3-positions, set to nullptr if nothing shall prepared
    ///	@param  col1        [in] Pointer to array with vec3-diffuse colors, set to nullptr if nothing shall prepared
    /// @return The allocated buffer data.
    static RenderBackend::BufferData *allocVertices(RenderBackend::VertexType type, ui32 numVerts, ::glm::vec3 *pos, 
            ::glm::vec3 *col1, ::glm::vec2 *tex0, RenderBackend::BufferAccessType access );

    static void updateTextVertices( ui32 numVerts, ::glm::vec2 *tex0, RenderBackend::BufferData *vb );

    /// No copying.
    GeometryBuilder( const GeometryBuilder& ) = delete;
    GeometryBuilder &operator = ( const GeometryBuilder& ) = delete;
};

} // Namespace Scene
} // namespace OSRE
