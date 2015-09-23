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
#include <osre/Scene/GeometryBuilder.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

const String VsSrc =
    "#version 400 core\n"
    "\n"
    "layout(location = 0) in vec3 position;	      // object space vertex position\n"
    "layout(location = 1) in vec3 normal;	            // object space vertex normal\n"
    "layout(location = 2) in vec3 color0;  // per-vertex colour\n"
    "\n"
    "// output from the vertex shader\n"
    "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
    "\n"
    "// uniform\n"
    "uniform mat4 MVP;	//combined modelview projection matrix\n"
    "\n"
    "void main()\n"
    "{\n"
    "    //assign the per-vertex color to vSmoothColor varying\n"
    "    vSmoothColor = vec4(color0,1);\n"
    "\n"
    "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    //vertex position\n"
    "    gl_Position = MVP*vec4(position,1);\n"
    "}\n";

const String FsSrc =
    "#version 400 core\n"
    "\n"
    "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
    "\n"
    "//input form the vertex shader\n"
    "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
    "\n"
    "void main()\n"
    "{\n"
    "    //set the interpolated color as the shader output\n"
    "    vFragColor = vSmoothColor;\n"
    "}\n";


static const String TextVsSrc =
	"layout(location = 0) in vec3 position;	      // object space vertex position\n"
	"layout(location = 1) in vec3 normal;	            // object space vertex normal\n"
	"layout(location = 2) in vec2 texcoord0;	        // texture coordinate\n"
	"out vec2 vUV;\n"
	"\n"
	"void main() {\n"
	"    vec3 vertexPosition_homoneneousspace = position;\n"
	"    gl_Position = vec4(position, 1 );\n"
	"    // UV of the vertex. No special space for this one.\n"
	"    UV = texcoord0;\n"
	"};\n";

static const String TextFsSrc =
	"in vec2 vUV;\n"
	"// Output data\n"
	"out vec4 vFragColor;\n"
	"uniform sampler2D tex0;\n"

	"void main() {\n"
	"    vFragColor = texture( tex0, vUV );\n"
	"};\n";

//-------------------------------------------------------------------------------------------------
GeometryBuilder::GeometryBuilder() {
    // empty
}

//-------------------------------------------------------------------------------------------------
GeometryBuilder::~GeometryBuilder() {
    // empty
}

//-------------------------------------------------------------------------------------------------
RenderBackend::Geometry *GeometryBuilder::createEmptyGeometry( RenderBackend::VertexType type ) {
    Geometry *geo = new Geometry;
    geo->m_vertextype = type;
    geo->m_indextype = UnsignedShort;

    return geo;
}

//-------------------------------------------------------------------------------------------------
RenderBackend::Geometry *GeometryBuilder::createTriangle() {
    Geometry *geo = new Geometry;
    geo->m_vertextype = ColorVertex;
    geo->m_indextype = UnsignedShort;

    // setup triangle vertices    
    static const ui32 NumVert = 3;
    ColorVert vertices[ NumVert ];
    vertices[ 0 ].color = glm::vec3( 1, 0, 0 );
    vertices[ 1 ].color = glm::vec3( 0, 1, 0 );
    vertices[ 2 ].color = glm::vec3( 0, 0, 1 );

    vertices[ 0 ].position = glm::vec3( -1, -1, 0 );
    vertices[ 1 ].position = glm::vec3( 0, 1, 0 );
    vertices[ 2 ].position = glm::vec3( 1, -1, 0 );

    ui32 size( sizeof( ColorVert ) * NumVert );
    geo->m_vb = BufferData::alloc( VertexBuffer, size, ReadOnly );
    ::memcpy( geo->m_vb->m_pData, vertices, size );

    // setup triangle indices
    static const ui32 NumIndices = 3;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;
    
    size = sizeof( GLushort ) * NumIndices;
    geo->m_ib = BufferData::alloc( IndexBuffer, size, ReadOnly );
    ::memcpy( geo->m_ib->m_pData, indices, size );

	// setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups   = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups[ 0 ].m_indexType     = UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 3 * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive     = TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex    = 0;

	// setup material
    geo->m_material = new Material;
    geo->m_material->m_numTextures = 0;
    geo->m_material->m_type = ShaderMaterial;
    geo->m_material->m_pShader = new Shader;
    geo->m_material->m_pShader->m_src[ SH_VertexShaderType ] = VsSrc;
    geo->m_material->m_pShader->m_src[ SH_FragmentShaderType ] = FsSrc;

	// setup shader attributes and variables
    if( nullptr != geo->m_material->m_pShader ) {
        ui32 numAttribs( ColorVert::getNumAttributes() );
        const String *attribs( ColorVert::getAttributes() );
        geo->m_material->m_pShader->m_attributes.add(attribs, numAttribs);
        geo->m_material->m_pShader->m_parameters.add( "MVP" );
    }

    return geo;
}

//-------------------------------------------------------------------------------------------------
RenderBackend::Geometry *GeometryBuilder::createBox( f32 w, f32 h, f32 d ) {
    Geometry *pGeometry = new Geometry;
    pGeometry->m_vertextype = ColorVertex;
    pGeometry->m_indextype  = UnsignedShort;

    static const ui32 NumVert = 8;

    ColorVert vertices[ NumVert ];
    f32 x(0.0f), y(0.0f), z( 0.0f );
    vertices[ 0 ].position = glm::vec3( x,     y,     z );
    vertices[ 1 ].position = glm::vec3( x + w, y,     z );
    vertices[ 2 ].position = glm::vec3( x + w, y + h, z );
    vertices[ 3 ].position = glm::vec3( x,     y + h, z );

    vertices[ 4 ].position = glm::vec3( x,     y,     z + d );
    vertices[ 5 ].position = glm::vec3( x + w, y,     z + d );
    vertices[ 6 ].position = glm::vec3( x + w, y + h, z + d );
    vertices[ 7 ].position = glm::vec3( x,     y + h, z + d );

    ui32 size( sizeof( ColorVert ) * NumVert );
    pGeometry->m_vb = BufferData::alloc( VertexBuffer, size, ReadOnly );
    ::memcpy( pGeometry->m_vb->m_pData, vertices, size );

    static const ui32 NumIndices = 36;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;
    indices[ 3 ] = 0+3;
    indices[ 4 ] = 1+3;
    indices[ 5 ] = 2+3;

    indices[ 6 ] = 0+6;
    indices[ 7 ] = 1+6;
    indices[ 8 ] = 2+6;
    indices[ 9 ] = 0+9;
    indices[ 10 ] = 1+9;
    indices[ 11 ] = 2+9;

    indices[ 12 ] = 0+12;
    indices[ 13 ] = 1+12;
    indices[ 14 ] = 2+12;
    indices[ 15 ] = 0+15;
    indices[ 16 ] = 1+15;
    indices[ 17 ] = 2+15;

    indices[ 18 ] = 0+18;
    indices[ 19 ] = 1 + 18;
    indices[ 20 ] = 2 + 18;
    indices[ 21 ] = 0+21;
    indices[ 22 ] = 1 + 21;
    indices[ 23 ] = 2 + 21;

    indices[ 24 ] = 0+24;
    indices[ 25 ] = 1 + 24;
    indices[ 26 ] = 2 + 24;
    indices[ 27 ] = 0+27;
    indices[ 28 ] = 1 + 27;
    indices[ 29 ] = 2 + 27;

    indices[ 30 ] = 0+30;
    indices[ 31 ] = 1 + 30;
    indices[ 32 ] = 2 + 30;
    indices[ 33 ] = 0+33;
    indices[ 34 ] = 1 + 33;
    indices[ 35 ] = 2 + 33;
    size = sizeof( GLushort ) * NumIndices;
    pGeometry->m_ib = BufferData::alloc( IndexBuffer, size, ReadOnly );
    ::memcpy( pGeometry->m_ib->m_pData, indices, size );

    return pGeometry;
}

//-------------------------------------------------------------------------------------------------
RenderBackend::Geometry *GeometryBuilder::createTextBox( f32 x, f32 y, f32 size, const String &text ) {
	if ( text.empty() ) {
		return nullptr;
	}

	Geometry *geo = new Geometry;
	std::vector<RenderVert> vertices;
	std::vector<GLushort> indices;
	for (ui32 i = 0; i <text.size(); i++) {
		RenderVert vert[ 4 ];
		vert[ 0 ].position = glm::vec3( x + i*size, y + size, 0 );
		vert[ 1 ].position = glm::vec3( x + i*size + size, y + size, 0 );
		vert[ 2 ].position = glm::vec3( x + i*size + size, y, 0 );
		vert[ 3 ].position = glm::vec3( x + i*size, y, 0 );

		char character = text[ i ];
		float uv_x = ( character % 16 ) / 16.0f;
		float uv_y = ( character / 16 ) / 16.0f;
		vert[ 0 ].tex0 = glm::vec2( uv_x, 1.0f - uv_y );
		vert[ 1 ].tex0 = glm::vec2( uv_x + 1.0f / 16.0f, 1.0f - uv_y );
		vert[ 2 ].tex0 = glm::vec2( uv_x + 1.0f / 16.0f, 1.0f - ( uv_y + 1.0f / 16.0f ) );
		vert[ 3 ].tex0 = glm::vec2( uv_x, 1.0f - ( uv_y + 1.0f / 16.0f ) );

		for (ui32 j = 0; j < 4; j++) {
			vertices.push_back( vert[ j ] );
		}

		// setup indices
		static const ui32 NumIndices = 6;
		GLushort  quad[ NumIndices ];
		quad[ 0 ] = 0;
		quad[ 1 ] = 1;
		quad[ 2 ] = 2;

		quad[ 0 ] = 2;
		quad[ 1 ] = 3;
		quad[ 2 ] = 1;

		for (ui32 j = 0; j < 6; ++j) {
			indices.push_back( quad[ i ] + (i*4) );
		}
	}

	const ui32 vertSize( vertices.size() * sizeof( RenderVert ) );
	BufferData *bufferData = BufferData::alloc( VertexBuffer, vertSize, ReadWrite );
	::memcpy( bufferData->m_pData, &vertices[ 0 ], vertSize );
	geo->m_vb = bufferData;

	size = sizeof( GLushort ) * text.size() * 6;
	geo->m_ib = BufferData::alloc( IndexBuffer, size, ReadOnly );
	::memcpy( geo->m_ib->m_pData, &indices[ 0 ], size );

	geo->m_numPrimGroups = 1;
	geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
	geo->m_pPrimGroups[ 0 ].m_indexType = UnsignedShort;
	geo->m_pPrimGroups[ 0 ].m_numPrimitives = text.size()*2;
	geo->m_pPrimGroups[ 0 ].m_primitive = TriangleList;
	geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

	// setup material
	geo->m_material = new Material;
	geo->m_material->m_numTextures = 0;
	geo->m_material->m_type = ShaderMaterial;
	geo->m_material->m_pShader = new Shader;
	geo->m_material->m_pShader->m_src[ SH_VertexShaderType ] = VsSrc;
	geo->m_material->m_pShader->m_src[ SH_FragmentShaderType ] = FsSrc;

	// setup shader attributes and variables
	if (nullptr != geo->m_material->m_pShader) {
		ui32 numAttribs( ColorVert::getNumAttributes() );
		const String *attribs( ColorVert::getAttributes() );
		geo->m_material->m_pShader->m_attributes.add( attribs, numAttribs );
		geo->m_material->m_pShader->m_parameters.add( "MVP" );
	}

	return geo;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Scene
} // namespace OSRE
