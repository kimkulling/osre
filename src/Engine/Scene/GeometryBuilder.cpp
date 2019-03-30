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
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Common/Logger.h>
#include <osre/Common/Tokenizer.h>
#include <osre/Debugging/osre_debugging.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <sstream>
#include <iostream>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

static const c8 *Tag = "GeometryBuilder";

const String GLSLVersionString_400 = "#version 400 core\n";

const String TextVsSrc =
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

const String TextFsSrc =
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

void MeshDiagnostic::dumpTextBox( ui32 i, glm::vec3 *textPos, ui32 VertexOffset ) {
    std::stringstream stream;
    stream << std::endl;
    stream << "i = " << i << " : " << textPos[ VertexOffset + 0 ].x << ", " << textPos[ VertexOffset + 0 ].y << ", " << textPos[ VertexOffset + 0 ].z << "\n";
    stream << "i = " << i << " : " << textPos[ VertexOffset + 1 ].x << ", " << textPos[ VertexOffset + 1 ].y << ", " << textPos[ VertexOffset + 1 ].z << "\n";
    stream << "i = " << i << " : " << textPos[ VertexOffset + 2 ].x << ", " << textPos[ VertexOffset + 2 ].y << ", " << textPos[ VertexOffset + 2 ].z << "\n";
    stream << "i = " << i << " : " << textPos[ VertexOffset + 3 ].x << ", " << textPos[ VertexOffset + 3 ].y << ", " << textPos[ VertexOffset + 3 ].z << "\n";
    osre_info( Tag, stream.str() );
}

void MeshDiagnostic::dumpTextTex0Box( ui32 i, glm::vec2 *tex0Pos, ui32 VertexOffset ) {
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

void MeshDiagnostic::dumpVertices( RenderBackend::RenderVert *renderVertices, ui32 numVertices ) {
    if ( 0 == numVertices || nullptr == renderVertices ) {
        return;
    }

    for ( ui32 i = 0; i < numVertices; i++ ) {
        dumpRenderVertex( i, renderVertices[ i ] );
    }
}

void MeshDiagnostic::dumpVertices(const CPPCore::TArray<RenderBackend::RenderVert> &renderVertices) {
	if ( renderVertices.isEmpty() ) {
		return;
	}
	for (ui32 i = 0; i < renderVertices.size(); i++ ) {
        dumpRenderVertex( i, renderVertices[ i ] );
	}
}

void MeshDiagnostic::dumpIndices(const CPPCore::TArray<ui16> &indexArray) {
	if ( indexArray.isEmpty() ) {
		return;
	}
    std::stringstream stream;
    stream << std::endl;

	for (ui32 i = 0; i<indexArray.size(); i++) {
		std::cout << indexArray[i] << ", ";
	}
	std::cout << "\n";
}

void MeshDiagnostic::dumpIndices( ui16 *indices, ui32 numIndices ) {
    if ( nullptr == indices || 0 == numIndices ) {
        return;
    }

    std::stringstream stream;
    stream << std::endl;
    for ( ui32 i = 0; i < numIndices; i++ ) {
        stream << indices[ i ] << ", ";
    }
    stream << "\n";
    osre_info( Tag, stream.str() );
}

void MeshDiagnostic::dumpIndices( const CPPCore::TArray<ui32> &indexArray ) {
    if ( indexArray.isEmpty() ) {
        return;
    }

    for ( ui32 i = 0; i < indexArray.size(); i++ ) {
        std::cout << indexArray[ i ] << ", ";
    }
    std::cout << "\n";

}

MeshBuilder::MeshBuilder() 
: m_ActiveGeo( nullptr )
, m_isDirty( false ) {
    // empty
}

MeshBuilder::~MeshBuilder() {
    // empty
}

void MeshBuilder::allocEmptyMesh( VertexType type, ui32 numMeshes ) {
    m_ActiveGeo = Mesh::create(numMeshes);
    for (ui32 i = 0; i < numMeshes; ++i) {
        m_ActiveGeo[ i ].m_vertextype = type;
        m_ActiveGeo[ i ].m_indextype = IndexType::UnsignedShort;
    }
}

void MeshBuilder::allocTriangles( VertexType type, BufferAccessType access ) {
    Mesh *geo = Mesh::create( 1 );
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
    ::memcpy( geo->m_ib->getData(), indices, size );

	// setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_primGroups   = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_primGroups[ 0 ].m_indexType     = IndexType::UnsignedShort;
    geo->m_primGroups[ 0 ].m_numIndices = 3 * geo->m_numPrimGroups;
    geo->m_primGroups[ 0 ].m_primitive     = PrimitiveType::TriangleList;
    geo->m_primGroups[ 0 ].m_startIndex    = 0;

	// setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    m_ActiveGeo = geo;
}

void MeshBuilder::allocQuads( VertexType type, BufferAccessType access ) {
    Mesh *geo = Mesh::create( 1 );
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
    ::memcpy( geo->m_ib->getData(), indices, size );

    // setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_primGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_primGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    geo->m_primGroups[ 0 ].m_numIndices = NumIndices * geo->m_numPrimGroups;
    geo->m_primGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    geo->m_primGroups[ 0 ].m_startIndex = 0;

    // setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    m_ActiveGeo = geo;
}

void MeshBuilder::allocUiQuad( const Rect2ui &dim, UiVertexCache &vc, RenderBackend::UiIndexCache &ic ) {
    const f32 x = dim.getX1();
    const f32 y = dim.getY1();
    const f32 w = dim.getWidth();
    const f32 h = dim.getHeight();

    // setup triangle vertices    
    static const ui32 NumVert = 4;
    glm::vec3 col[ NumVert ];
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );
    col[ 3 ] = glm::vec3( 1, 0, 0 );

    glm::vec3 pos[ NumVert ];
    pos[ 0 ] = glm::vec3( x, y, 0 );
    pos[ 1 ] = glm::vec3( x, y+h, 0 );
    pos[ 2 ] = glm::vec3( x+w, y, 0 );
    pos[ 3 ] = glm::vec3( x+h, y+h, 0 );

    glm::vec2 tex0[ NumVert ];
    tex0[ 0 ] = glm::vec2( 0, 0 );
    tex0[ 1 ] = glm::vec2( 0, 1 );
    tex0[ 2 ] = glm::vec2( 1, 0 );
    tex0[ 3 ] = glm::vec2( 1, 1 );

    for (ui32 i = 0; i < 4; ++i) {
        RenderVert v;
        v.position = pos[ i ];
        v.color0 = col[ i ];
        v.tex0 = tex0[ i ];
        vc.add( v );
    }

    // setup triangle indices
    static const ui32 NumIndices = 6;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 2;
    indices[ 2 ] = 1;

    indices[ 3 ] = 1;
    indices[ 4 ] = 2;
    indices[ 5 ] = 3;
    
    for (ui32 i = 0; i < 4; ++i) {
        ic.add( indices[ i ] );
    }
}

void MeshBuilder::allocCube( RenderBackend::VertexType type, RenderBackend::BufferAccessType access ) {
    m_ActiveGeo = Mesh::create( 1 );    
}

void MeshBuilder::allocLineList( VertexType type, BufferAccessType access, ui32 numLines, 
                                          glm::vec3 *posArray, glm::vec3 *colorArray, ui32 *indices ) {
    Mesh *geo = Mesh::create( 1 );
    geo->m_vertextype = type;
    geo->m_indextype = IndexType::UnsignedShort;

    geo->m_numPrimGroups = 1;
    geo->m_primGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_primGroups[ 0 ].init( IndexType::UnsignedShort, 2 * numLines, PrimitiveType::LineList, 0 );

    geo->m_vb = Scene::MeshBuilder::allocVertices( type, numLines, posArray, colorArray, nullptr, access );
    geo->m_indextype = IndexType::UnsignedShort;
    ui32 size = sizeof( GLushort ) * numLines*2;
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    geo->m_ib->copyFrom( indices, size );

    m_ActiveGeo = geo;
}

void MeshBuilder::allocPoints( VertexType type, BufferAccessType access, ui32 numPoints, 
                                        glm::vec3 *posArray, glm::vec3 *colorArray ) {
    // colors
    CPPCore::TArray<ui16> indices;
    indices.resize( numPoints );
    for ( ui16 i = 0; i < numPoints; i++ ) {
        indices[ i ] = i;
    }

    Mesh *ptGeo = Mesh::create( 1 );
    ptGeo->m_vertextype = type;

    ptGeo->m_vb = Scene::MeshBuilder::allocVertices( VertexType::ColorVertex, numPoints, posArray, 
                        colorArray, nullptr, access );
    ptGeo->m_indextype = IndexType::UnsignedShort;
    ui32 pt_size = sizeof( GLushort ) * numPoints;
    ptGeo->m_ib = BufferData::alloc( BufferType::IndexBuffer, pt_size, access );
    ptGeo->m_ib->copyFrom( &indices[0], pt_size );

    // setup primitives
    ptGeo->m_numPrimGroups = 1;
    ptGeo->m_primGroups = new PrimitiveGroup[ ptGeo->m_numPrimGroups ];
    ptGeo->m_primGroups[ 0 ].init( IndexType::UnsignedShort, 3, PrimitiveType::PointList, 0 );

    // setup material
    ptGeo->m_material = MaterialBuilder::createBuildinMaterial( type );;

    m_ActiveGeo = ptGeo;
}

static const ui32 NumQuadVert = 4;

static ui32 getNumTextVerts( const String &text ) {
    const ui32 NumTextVerts = NumQuadVert * text.size();
    return NumTextVerts;
}

static const ui32 NumQuadIndices = 6;

static ui32 getNumTextIndices(const String &text) {
    const ui32 numIndices = NumQuadIndices * text.size();
    return numIndices;
}

static void generateTextBoxVerticesAndIndices(f32 x, f32 y, f32 textSize, const String &text, 
        glm::vec3 **textPos, glm::vec3 **colors, glm::vec2 **tex0, GLushort **textIndices) {
    OSRE_ASSERT(nullptr != textPos);
    OSRE_ASSERT(nullptr != colors);
    OSRE_ASSERT(nullptr != tex0);
    OSRE_ASSERT(nullptr != textIndices);

    glm::vec3 col[NumQuadVert];
    col[0] = glm::vec3(0, 0, 0);
    col[1] = glm::vec3(0, 0, 0);
    col[2] = glm::vec3(0, 0, 0);
    col[3] = glm::vec3(0, 0, 0);

    glm::vec3 pos[NumQuadVert];
    pos[0] = glm::vec3(x, y, 0);
    pos[1] = glm::vec3(x, y + textSize, 0);
    pos[2] = glm::vec3(x + textSize, y, 0);
    pos[3] = glm::vec3(x + textSize, y + textSize, 0);

    const ui32 NumTextVerts = getNumTextVerts(text);
    *textPos = new glm::vec3[NumTextVerts];
    *colors = new glm::vec3[NumTextVerts];
    *tex0 = new glm::vec2[NumTextVerts];
    *textIndices = new GLushort[getNumTextIndices(text)];

    const f32 invCol = 1.f / 16.f;
    const f32 invRow = 1.f / 16.f;
    ui32 textCol(0), textRow(0);
    for (ui32 i = 0; i < text.size(); i++) {
        const c8 ch = text[i];
        if (Tokenizer::isLineBreak(ch)) {
            textCol = 0;
            textRow++;
            continue;
        }

        const ui16 VertexOffset(static_cast<ui16>(i) * static_cast<ui16>(NumQuadVert));
        const f32  rowHeight(-1.0f * textRow * textSize);
        (*textPos)[VertexOffset + 0].x = pos[0].x + (textCol*textSize);
        (*textPos)[VertexOffset + 0].y = pos[0].y + rowHeight;
        (*textPos)[VertexOffset + 0].z = 0;

        (*textPos)[VertexOffset + 1].x = pos[1].x + (textCol*textSize);
        (*textPos)[VertexOffset + 1].y = pos[1].y + rowHeight;
        (*textPos)[VertexOffset + 1].z = 0;

        (*textPos)[VertexOffset + 2].x = pos[2].x + (textCol*textSize);
        (*textPos)[VertexOffset + 2].y = pos[2].y + rowHeight;
        (*textPos)[VertexOffset + 2].z = 0;

        (*textPos)[VertexOffset + 3].x = pos[3].x + (textCol*textSize);
        (*textPos)[VertexOffset + 3].y = pos[3].y + rowHeight;
        (*textPos)[VertexOffset + 3].z = 0;

        //GeometryDiagnosticUtils::dumpTextBox( i, textPos, VertexOffset );

        const i32 column = (ch) % 16;
        const i32 row = (ch) / 16;
        const f32 s = column * invCol;
        const f32 t = (row + 1) * invRow;

        (*tex0)[VertexOffset + 0].x = s;
        (*tex0)[VertexOffset + 0].y = 1.0f - t;

        (*tex0)[VertexOffset + 1].x = s;
        (*tex0)[VertexOffset + 1].y = 1.0f - t + 1.0f / 16.0f;

        (*tex0)[VertexOffset + 2].x = s + 1.0f / 16.0f;
        (*tex0)[VertexOffset + 2].y = 1.0f - t;

        (*tex0)[VertexOffset + 3].x = s + 1.0f / 16.0f;
        (*tex0)[VertexOffset + 3].y = 1.0f - t + 1.0f / 16.0f;

        //GeometryDiagnosticUtils::dumpTextTex0Box(i, tex0, VertexOffset);
        (*colors)[VertexOffset + 0] = col[0];
        (*colors)[VertexOffset + 1] = col[1];
        (*colors)[VertexOffset + 2] = col[2];
        (*colors)[VertexOffset + 3] = col[3];
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

void MeshBuilder::allocTextBox( f32 x, f32 y, f32 textSize, const String &text, BufferAccessType access ) {
	if ( text.empty() ) {
		return;
	}

    Mesh *geo = Mesh::create( 1 );
    geo->m_vertextype = VertexType::RenderVertex;
    geo->m_indextype = IndexType::UnsignedShort;

    glm::vec3 *textPos( nullptr ), *colors(nullptr );
    glm::vec2 *tex0(nullptr);
    GLushort *textIndices(nullptr);
    generateTextBoxVerticesAndIndices(x,y,textSize, text, &textPos, &colors, &tex0, &textIndices);

    //GeometryDiagnosticUtils::dumpIndices( textIndices, 6 * text.size() );

    geo->m_vb = allocVertices( geo->m_vertextype, text.size() * NumQuadVert, textPos, colors, tex0, access );

    // setup triangle indices
    ui32 size = sizeof( GLushort ) * 6 * text.size();
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    geo->m_ib->copyFrom( textIndices, size );

    // setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_primGroups = new PrimitiveGroup[ 1 ];
    geo->m_primGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    geo->m_primGroups[ 0 ].m_numIndices = 6 * text.size();
    geo->m_primGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    geo->m_primGroups[ 0 ].m_startIndex = 0;

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

    m_ActiveGeo = geo;
}

void MeshBuilder::allocUiTextBox(f32 x, f32 y, f32 textSize, const String &text, BufferAccessType access, 
        UiVertexCache &vc, UiIndexCache &ic) {
    glm::vec3 *textPos(nullptr), *colors(nullptr);
    glm::vec2 *tex0(nullptr);
    GLushort *textIndices(nullptr);
    generateTextBoxVerticesAndIndices(x, y, textSize, text, &textPos, &colors, &tex0, &textIndices);
    for (ui32 i = 0; i < text.size(); i++) {
        RenderVert v;
        v.position = textPos[i];
        v.color0 = colors[i];
        v.tex0 = tex0[i];
        vc.add(v);
    }

    const ui32 numIndices(getNumTextIndices(text));
    for (ui32 i = 0; i < numIndices; ++i) {
        ic.add(textIndices[i]);
    }

    delete[] textIndices;
    delete[] tex0;
    delete[] textPos;
}

void MeshBuilder::updateTextBox( Mesh *geo, f32 textSize, const String &text ) {
    if ( nullptr == geo ) {
        osre_debug( Tag, "Pointer to geometry is nullptr." );
        return;
    }

    const ui32 numTextVerts( getNumTextVerts( text ) );
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
        if ( Tokenizer::isLineBreak( ch ) ) {
            textCol = 0;
            ++textRow;
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

BufferData *MeshBuilder::allocVertices( VertexType type, ui32 numVerts, glm::vec3 *pos, 
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
            ::memcpy( data->getData(), colVerts, size );
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
            ::memcpy( data->getData(), renderVerts, size );
            delete [] renderVerts;
        }
        break;

        default:
            break;
    }

    return data;
}

void MeshBuilder::updateTextVertices( ui32 numVerts, ::glm::vec2 *tex0, BufferData *vb ) {
    if ( nullptr == tex0 || nullptr == vb ) {
        return;
    }

    RenderVert *vert = new RenderVert[ numVerts ];
    ::memcpy( &vert[ 0 ].position, vb->getData(), vb->getSize() );
    for ( ui32 i = 0; i < numVerts; i++ ) {
        vert[ i ].tex0 = tex0[ i ];
    }
    ::memcpy( vb->getData(), vert, vb->getSize() );
    delete[] vert;
}

RenderBackend::Mesh *MeshBuilder::getMesh() {
    return m_ActiveGeo;
}

} // Namespace Scene
} // Namespace OSRE
