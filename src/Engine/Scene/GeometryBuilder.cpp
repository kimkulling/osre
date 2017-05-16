/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/MaterialBuilder.h>
#include <osre/Common/Logger.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <sstream>
#include <iostream>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

static const String Tag = "GeometryBuilder";

static const String GLSLVersionString_400 = "#version 400 core\n";

static const String TextVsSrc =
    GLSLVersionString_400 +
    "\n"
    "layout(location = 0) in vec3 position;	  // object space vertex position\n"
    "layout(location = 1) in vec3 normal;	  // object space vertex normal\n"
    "layout(location = 2) in vec3 color0;     // per-vertex colour\n"
    "layout(location = 3) in vec2 texcoord0;  // per-vertex colour\n"
    "\n"
    "smooth out vec4 vSmoothColor;		      // smooth colour to fragment shader\n"
    "out vec2 vUV;\n"
    "\n"
    "uniform mat4 MVP;	//combined modelview projection matrix\n"
	"\n"
	"void main() {\n"
    "    vSmoothColor = vec4(color0,1);\n"
    "\n"
	"    gl_Position = MVP*vec4( position, 1 );\n"
	"    // UV of the vertex. No special space for this one.\n"
	"    vUV = texcoord0;\n"
	"};\n";

static const String TextFsSrc =
    "#version 400 core\n"
    "\n"
    "in vec2 UV;\n"
	"// Output data\n"
    "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
    "smooth in vec2 vUV;\n"

	"out vec4 vFragColor;\n"
	"uniform sampler2D tex0;\n"

	"void main() {\n"
    "//    vFragColor = vec4(1,1,1,1 );\n"
    "    vFragColor = texture( tex0, UV );\n"
	"};\n";

void GeometryDiagnosticUtils::dumpTextBox( ui32 i, glm::vec3 *textPos, ui32 VertexOffset ) {
    std::stringstream stream;
    stream << std::endl;
    stream << "i = " << i << " : " << textPos[ VertexOffset + 0 ].x << ", " << textPos[ VertexOffset + 0 ].y << "\n";
    stream << "i = " << i << " : " << textPos[ VertexOffset + 1 ].x << ", " << textPos[ VertexOffset + 1 ].y << "\n";
    stream << "i = " << i << " : " << textPos[ VertexOffset + 2 ].x << ", " << textPos[ VertexOffset + 2 ].y << "\n";
    stream << "i = " << i << " : " << textPos[ VertexOffset + 3 ].x << ", " << textPos[ VertexOffset + 3 ].y << "\n";
    osre_info( Tag, stream.str() );
}

void GeometryDiagnosticUtils::dumpTextTex0Box( ui32 i, glm::vec2 *tex0Pos, ui32 VertexOffset ) {
    std::stringstream stream;
    stream << std::endl;
    stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 0 ].x << ", " << tex0Pos[ VertexOffset + 0 ].y << "\n";
    stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 1 ].x << ", " << tex0Pos[ VertexOffset + 1 ].y << "\n";
    stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 2 ].x << ", " << tex0Pos[ VertexOffset + 2 ].y << "\n";
    stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 3 ].x << ", " << tex0Pos[ VertexOffset + 3 ].y << "\n";
    osre_info( Tag, stream.str() );
}

static void dumpRenderVertex( ui32 idx, const RenderBackend::RenderVert &vertex ) {
    std::cout << "v[" << idx << "].position = " << vertex.position.x << "|" << vertex.position.y << "|" << vertex.position.z << "\n";
    std::cout << "v[" << idx << "].normal   = " << vertex.normal.x   << "|" << vertex.normal.y   << "|" << vertex.normal.z   << "\n";
    std::cout << "v[" << idx << "].color0   = " << vertex.color0.x   << "|" << vertex.color0.y   << "|" << vertex.color0.z   << "\n";
    std::cout << "v[" << idx << "].tex0     = " << vertex.tex0.x     << "|" << vertex.tex0.y     << "\n";
}

void GeometryDiagnosticUtils::dumVertices( RenderBackend::RenderVert *renderVertices, ui32 numVertices ) {
    if ( 0 == numVertices || nullptr == renderVertices ) {
        return;
    }

    for ( ui32 i = 0; i < numVertices; i++ ) {
        dumpRenderVertex( i, renderVertices[ i ] );
    }
}

void GeometryDiagnosticUtils::dumVertices(const CPPCore::TArray<RenderBackend::RenderVert> &renderVertices) {
	if ( renderVertices.isEmpty() ) {
		return;
	}
	for (ui32 i = 0; i < renderVertices.size(); i++ ) {
        dumpRenderVertex( i, renderVertices[ i ] );
	}
}

void GeometryDiagnosticUtils::dumpIndices(const CPPCore::TArray<ui16> &indexArray) {
	if ( indexArray.isEmpty() ) {
		return;
	}

	for (ui32 i = 0; i<indexArray.size(); i++) {
		std::cout << indexArray[i] << ", ";
	}
	std::cout << "\n";
}

GeometryBuilder::GeometryBuilder() {
    // empty
}

GeometryBuilder::~GeometryBuilder() {
    // empty
}

Geometry *GeometryBuilder::allocEmptyGeometry( VertexType type, ui32 numGeo ) {
    Geometry *geo = Geometry::create( numGeo );
    for ( ui32 i = 0; i < numGeo; i++ ) {
        geo[ i ].m_vertextype = type;
        geo[ i ].m_indextype = IndexType::UnsignedShort;
    }

    return geo;
}

Geometry *GeometryBuilder::allocTriangles( VertexType type, BufferAccessType access ) {
    Geometry *geo = Geometry::create( 1 );
    geo->m_vertextype = type;
    geo->m_indextype = IndexType::UnsignedShort;

    // setup triangle vertices    
    static const ui32 NumVert = 3;
    glm::vec3 col[ NumVert ];
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );

    glm::vec3 pos[ NumVert ];
    pos[ 0 ] = glm::vec3( -1, -1, 0 );
    pos[ 1 ] = glm::vec3( 0, 1, 0 );
    pos[ 2 ] = glm::vec3( 1, -1, 0 );
    geo->m_vb = allocVertices( geo->m_vertextype,  NumVert, pos, col, nullptr, access );

    // setup triangle indices
    static const ui32 NumIndices = 3;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 2;
    indices[ 2 ] = 1;
    
    ui32 size = sizeof( GLushort ) * NumIndices;
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, access );
    ::memcpy( geo->m_ib->m_data, indices, size );

	// setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups   = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups[ 0 ].m_indexType     = IndexType::UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 3 * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive     = PrimitiveType::TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex    = 0;

	// setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    return geo;
}

Geometry *GeometryBuilder::allocQuads( VertexType type, BufferAccessType access ) {
    Geometry *geo = Geometry::create( 1 );
    geo->m_vertextype = type;
    geo->m_indextype = IndexType::UnsignedShort;

    // setup triangle vertices    
    static const ui32 NumVert = 4;
    glm::vec3 col[ NumVert ];
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );
    col[ 3 ] = glm::vec3( 1, 0, 0 );

    glm::vec3 pos[ NumVert ];
    pos[ 0 ] = glm::vec3( -1, -1, 0 );
    pos[ 1 ] = glm::vec3( -1, 1, 0 );
    pos[ 2 ] = glm::vec3( 1, -1, 0 );
    pos[ 3 ] = glm::vec3( 1, 1, 0 );

    glm::vec2 tex0[ NumVert ];
    tex0[ 0 ] = glm::vec2( 0, 0 );
    tex0[ 1 ] = glm::vec2( 0, 1 );
    tex0[ 2 ] = glm::vec2( 1, 0 );
    tex0[ 3 ] = glm::vec2( 1, 1 );

    geo->m_vb = allocVertices( geo->m_vertextype, NumVert, pos, col, tex0, access );

    // setup triangle indices
    static const ui32 NumIndices = 6;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 2;
    indices[ 2 ] = 1;

    indices[ 3 ] = 1;
    indices[ 4 ] = 2;
    indices[ 5 ] = 3;

    ui32 size = sizeof( GLushort ) * NumIndices;
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    ::memcpy( geo->m_ib->m_data, indices, size );

    // setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 6 * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    // setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    return geo;
}

Geometry *GeometryBuilder::allocLineList( VertexType type, BufferAccessType access, ui32 numLines, 
                                          glm::vec3 *posArray, glm::vec3 *colorArray, ui32 *indices ) {
    Geometry *geo = Geometry::create( 1 );
    geo->m_vertextype = type;
    geo->m_indextype = IndexType::UnsignedShort;

    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups[ 0 ].init( IndexType::UnsignedShort, 2 * numLines, PrimitiveType::LineList, 0 );

    geo->m_vb = Scene::GeometryBuilder::allocVertices( type, numLines, posArray, colorArray, nullptr, access );
    geo->m_indextype = IndexType::UnsignedShort;
    ui32 size = sizeof( GLushort ) * numLines*2;
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    geo->m_ib->copyFrom( indices, size );

    return geo;
}

Geometry *GeometryBuilder::allocPoints( VertexType type, BufferAccessType access, ui32 numPoints, 
                                        glm::vec3 *posArray, glm::vec3 *colorArray ) {
    // colors
    CPPCore::TArray<ui16> indices;
    indices.resize( numPoints );
    for ( ui16 i = 0; i < numPoints; i++ ) {
        indices[ i ] = i;
    }

    Geometry *ptGeo = GeometryBuilder::allocEmptyGeometry( type, 1 );

    ptGeo->m_vb = Scene::GeometryBuilder::allocVertices( VertexType::ColorVertex, numPoints, posArray, colorArray, nullptr, BufferAccessType::ReadOnly );
    ptGeo->m_indextype = IndexType::UnsignedShort;
    ui32 pt_size = sizeof( GLushort ) * numPoints;
    ptGeo->m_ib = BufferData::alloc( BufferType::IndexBuffer, pt_size, BufferAccessType::ReadOnly );
    ptGeo->m_ib->copyFrom( &indices[0], pt_size );

    // setup primitives
    ptGeo->m_numPrimGroups = 1;
    ptGeo->m_pPrimGroups = new PrimitiveGroup[ ptGeo->m_numPrimGroups ];
    ptGeo->m_pPrimGroups[ 0 ].init( IndexType::UnsignedShort, 3, PrimitiveType::PointList, 0 );

    // setup material
    Material *mat = MaterialBuilder::createBuildinMaterial( type );
    ptGeo->m_material = mat;

    return ptGeo;
}

static bool isLineBreak(c8 c) {
    if (c == '\n') {
        return true;
    } else {
        return false;
    }
}

static const ui32 NumQuadVert = 4;

static ui32 getNumTextVerts( const String &text ) {
    const ui32 NumTextVerts = NumQuadVert * text.size();
    return NumTextVerts;
}

Geometry *GeometryBuilder::allocTextBox( f32 x, f32 y, f32 textSize, const String &text, BufferAccessType access ) {
	if ( text.empty() ) {
		return nullptr;
	}

    Geometry *geo = Geometry::create( 1 );
    geo->m_vertextype = VertexType::RenderVertex;
    geo->m_indextype = IndexType::UnsignedShort;

    // setup triangle vertices    
    glm::vec3 col[ NumQuadVert ];
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );
    col[ 3 ] = glm::vec3( 1, 0, 0 );

    glm::vec3 pos[ NumQuadVert ];
    pos[ 0 ] = glm::vec3( 0, 0, 0 );
    pos[ 1 ] = glm::vec3( 0, textSize, 0 );
    pos[ 2 ] = glm::vec3( textSize, 0, 0 );
    pos[ 3 ] = glm::vec3( textSize, textSize, 0 );

    static const ui32 NumQuadIndices = 6;
    GLushort  indices[ NumQuadIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;

    indices[ 3 ] = 2;
    indices[ 4 ] = 1;
    indices[ 5 ] = 3;

    const ui32 NumTextVerts = getNumTextVerts( text );
    glm::vec3 *textPos = new glm::vec3[ NumTextVerts ];
    glm::vec3 *colors = new glm::vec3[ NumTextVerts ];
	glm::vec2 *tex0 = new glm::vec2[ NumTextVerts ];
    GLushort *textIndices = new GLushort[ NumQuadIndices * text.size() ];

    const f32 invCol = 1.f / 16.f;
    const f32 invRow = 1.f / 16.f;
    ui32 textCol( 0 ), textRow( 0 );
    for (ui32 i = 0; i < text.size(); i++) {
        const c8 ch = text[ i ];
        if ( isLineBreak( ch ) ) {
            textCol = 0;
            textRow++;
            continue;
        }
        
        const ui16 VertexOffset(static_cast<ui16>( i ) * static_cast<ui16>( NumQuadVert ) );
        const f32  rowHeight( -1.0f * textRow * textSize );
        textPos[ VertexOffset + 0 ].x = pos[ 0 ].x + ( textCol*textSize );
        textPos[ VertexOffset + 0 ].y = pos[ 0 ].y + rowHeight;

        textPos[ VertexOffset + 1 ].x = pos[ 1 ].x + ( textCol*textSize );
        textPos[ VertexOffset + 1 ].y = pos[ 1 ].y + rowHeight;

        textPos[ VertexOffset + 2 ].x = pos[ 2 ].x + ( textCol*textSize );
        textPos[ VertexOffset + 2 ].y = pos[ 2 ].y + rowHeight;

        textPos[ VertexOffset + 3 ].x = pos[ 3 ].x + ( textCol*textSize );
        textPos[ VertexOffset + 3 ].y = pos[ 3 ].y + rowHeight;

        //dumpTextBox( i, textPos, VertexOffset );
        
        const i32 column = (ch ) % 16;
        const i32 row = (ch ) / 16;
        const f32 s = column * invCol;
        const f32 t = (row + 1) * invRow;

		tex0[VertexOffset + 0].x = s;
		tex0[VertexOffset + 0].y = 1.0f - t;

		tex0[VertexOffset + 1].x = s;
        tex0[VertexOffset + 1].y = 1.0f - t + 1.0f / 16.0f;

		tex0[VertexOffset + 2].x = s + 1.0f/16.0f;
        tex0[VertexOffset + 2].y = 1.0f - t;
       
        tex0[VertexOffset + 3].x = s + 1.0f / 16.0f;
        tex0[VertexOffset + 3].y = 1.0f - t + 1.0f / 16.0f;
        
        //dumpTextTex0Box(i, tex0, VertexOffset);
        colors[ VertexOffset + 0 ] = col[ 0 ];
        colors[ VertexOffset + 1 ] = col[ 1 ];
        colors[ VertexOffset + 2 ] = col[ 2 ];
        colors[ VertexOffset + 3 ] = col[ 3 ];
        const ui32 IndexOffset( i * NumQuadIndices );
        textIndices[ 0 + IndexOffset ] = 0 + VertexOffset;
        textIndices[ 1 + IndexOffset ] = 2 + VertexOffset;
        textIndices[ 2 + IndexOffset ] = 1 + VertexOffset;

        textIndices[ 3 + IndexOffset ] = 1 + VertexOffset;
        textIndices[ 4 + IndexOffset ] = 2 + VertexOffset;
        textIndices[ 5 + IndexOffset ] = 3 + VertexOffset;
        textCol++;
    }

    geo->m_vb = allocVertices( geo->m_vertextype, text.size() * NumQuadVert, textPos, colors, tex0, access );

    // setup triangle indices
    ui32 size = sizeof( GLushort ) * 6 * text.size();
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    ::memcpy( geo->m_ib->m_data, textIndices, size );

    // setup primitives
    geo->m_numPrimGroups = text.size();
    geo->m_pPrimGroups = new PrimitiveGroup[ 1 ];
    geo->m_pPrimGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 6 * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    // setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( VertexType::RenderVertex );

    // setup the texture
    geo->m_material->m_numTextures = 1;
    geo->m_material->m_textures = new Texture*[ 1 ];
    geo->m_material->m_textures[ 0 ] = new Texture;
    geo->m_material->m_textures[ 0 ]->m_textureName = "buildin_arial";
    geo->m_material->m_textures[ 0 ]->m_loc = IO::Uri( "file://assets/Textures/Fonts/buildin_arial.bmp" );

    geo->m_material->m_textures[0]->m_targetType = TextureTargetType::Texture2D;
    geo->m_material->m_textures[0]->m_width = 0;
    geo->m_material->m_textures[0]->m_height = 0;
    geo->m_material->m_textures[0]->m_channels = 0;
    geo->m_material->m_textures[0]->m_data = nullptr;
    geo->m_material->m_textures[0]->m_size = 0;

    return geo;
}

void GeometryBuilder::updateTextBox( RenderBackend::Geometry *geo, f32 textSize, const String &text, bool resize ) {
    ui32 numTextVerts( getNumTextVerts( text ) );
    glm::vec2 *tex0 = new glm::vec2[ numTextVerts ];

    // setup triangle vertices    
    glm::vec3 col[ NumQuadVert ];
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );
    col[ 3 ] = glm::vec3( 1, 0, 0 );

    glm::vec3 pos[ NumQuadVert ];
    pos[ 0 ] = glm::vec3( 0, 0, 0 );
    pos[ 1 ] = glm::vec3( 0, textSize, 0 );
    pos[ 2 ] = glm::vec3( textSize, 0, 0 );
    pos[ 3 ] = glm::vec3( textSize, textSize, 0 );

    const f32 invCol = 1.f / 16.f;
    const f32 invRow = 1.f / 16.f;
    ui32 textCol( 0 ), textRow( 0 );
    for ( ui32 i = 0; i < text.size(); i++ ) {
        const c8 ch = text[ i ];
        if ( isLineBreak( ch ) ) {
            textCol = 0;
            textRow++;
            continue;
        }

        const ui32 VertexOffset( i * NumQuadVert );

        const i32 column = ( ch ) % 16;
        const i32 row = ( ch ) / 16;
        const f32 s = column * invCol;
        const f32 t = ( row + 1 ) * invRow;


        tex0[ VertexOffset + 0 ].x = s;
        tex0[ VertexOffset + 0 ].y = 1.0f - t;

        tex0[ VertexOffset + 1 ].x = s;
        tex0[ VertexOffset + 1 ].y = 1.0f - t + 1.0f / 16.0f;

        tex0[ VertexOffset + 2 ].x = s + 1.0f / 16.0f;
        tex0[ VertexOffset + 2 ].y = 1.0f - t;

        tex0[ VertexOffset + 3 ].x = s + 1.0f / 16.0f;
        tex0[ VertexOffset + 3 ].y = 1.0f - t + 1.0f / 16.0f;
    }

    updateTextVertices(  numTextVerts, tex0, geo->m_vb );
    delete[] tex0;
}

BufferData *GeometryBuilder::allocVertices( VertexType type, ui32 numVerts, glm::vec3 *pos, 
                                            glm::vec3 *col1, glm::vec2 *tex0, BufferAccessType access ) {
    BufferData *data( nullptr );
    ui32 size( 0 );
    switch (type) {
        case VertexType::ColorVertex: {
            ColorVert *colVerts = new ColorVert[ numVerts ];
            if (nullptr != pos) {
                for (ui32 i = 0; i < numVerts; i++) {
                    colVerts[ i ].position = pos[ i ];
                }
            }
            if (nullptr != col1 ) {
                for (ui32 i = 0; i < numVerts; i++) {
                    colVerts[ i ].color0 = col1[ i ];
                }
            }
            size = sizeof( ColorVert ) * numVerts;
            data = BufferData::alloc( BufferType::VertexBuffer, size, access );
            ::memcpy( data->m_data, colVerts, size );
            delete [] colVerts;
        }
        break;

        case VertexType::RenderVertex: {
            RenderVert *renderVerts = new RenderVert[ numVerts ];
            if (nullptr != pos) {
                for (ui32 i = 0; i < numVerts; i++) {
                    renderVerts[ i ].position = pos[ i ];
                }
            }
            if (nullptr != col1) {
                for (ui32 i = 0; i < numVerts; i++) {
                    renderVerts[ i ].color0 = col1[ i ];
                }
            }
            if (nullptr != tex0) {
                for (ui32 i = 0; i < numVerts; i++) {
                    renderVerts[ i ].tex0 = tex0[ i ];
                }
            }

            size = sizeof( RenderVert ) * numVerts;
            data = BufferData::alloc( BufferType::VertexBuffer, size, BufferAccessType::ReadOnly );
            ::memcpy( data->m_data, renderVerts, size );
            delete [] renderVerts;
        }
        break;

        default:
            break;
    }

    return data;
}

void GeometryBuilder::updateTextVertices( ui32 numVerts, ::glm::vec2 *tex0, BufferData *vb ) {
    if ( nullptr == tex0 || nullptr == vb ) {
        return;
    }

    RenderVert *vert = new RenderVert[ numVerts ];
    ::memcpy( &vert[ 0 ].position, vb->m_data, vb->m_size );
    for ( ui32 i = 0; i < numVerts; i++ ) {
        vert[ i ].tex0 = tex0[ i ];
    }
    ::memcpy( vb->m_data, vert, vb->m_size );
    delete[] vert;
}

} // Namespace Scene
} // Namespace OSRE
