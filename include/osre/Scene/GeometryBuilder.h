/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace OSRE {


namespace RenderBackend {
    struct Geometry;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Scene::GeometryBuilder
///	@ingroup	Application
///
///	@brief  This class is used to create the geometry for base shapes like a triangle.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT GeometryBuilder {
public:
    /// @brief  The class constructor.
    GeometryBuilder();

    /// @brief  The class destructor.
    ~GeometryBuilder();

    RenderBackend::Geometry *allocEmptyGeometry( RenderBackend::VertexType type );

    /// @brief  Will create a triangle geometry.
    /// @return The created geometry.
    RenderBackend::Geometry *allocTriangles( RenderBackend::VertexType type );

    ///	@brief
    RenderBackend::Geometry *allocQuads( RenderBackend::VertexType type );

    ///	@brief
	RenderBackend::Geometry *allocTextBox( f32 x, f32 y, f32 textSize, const String &text );

    ///	@brief  Allocates vertices into a buffer data.
    /// @param  type        [in] The vertex type to create.
    ///	@param  numVerts    [in] The number of vertices to create.
    ///	@param  pos         [in] Pointer to array with vec3-positions, set to nullptr if nothing shall prepared
    ///	@param  col1        [in] Pointer to array with vec3-diffuse colors, set to nullptr if nothing shall prepared
    static RenderBackend::BufferData *allocVertices( RenderBackend::VertexType type, ui32 numVerts, ::glm::vec3 *pos, ::glm::vec3 *col1 );

};

//-------------------------------------------------------------------------------------------------

} // Namespace Scene
} // namespace OSRE
