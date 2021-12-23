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
#include <osre/Scene/MeshBuilder.h>
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

#include <cstdio>

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

MeshBuilder::MeshBuilder() 
: mIsDirty( false )
, mActiveGeo( nullptr ) {
    // empty
}

MeshBuilder::~MeshBuilder() {
    // empty
}

MeshBuilder &MeshBuilder::allocEmptyMesh( VertexType type, ui32 numMeshes ) {
    mActiveGeo = Mesh::create(numMeshes, type);
    for (ui32 i = 0; i < numMeshes; ++i) {
        mActiveGeo[ i ].m_indextype = IndexType::UnsignedShort;
    }

    return *this;
}

MeshBuilder &MeshBuilder::allocTriangles( VertexType type, BufferAccessType access ) {
    Mesh *mesh = Mesh::create(1, VertexType::RenderVertex);
    mesh->m_vertextype = type;
    mesh->m_indextype = IndexType::UnsignedShort;

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
    mesh->m_vb = allocVertices( mesh->m_vertextype,  NumVert, pos, col, nullptr, access );

    // setup triangle indices
    static const size_t NumIndices = 3;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 2;
    indices[ 2 ] = 1;

    size_t size = sizeof( GLushort ) * NumIndices;
    mesh->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, access );
    ::memcpy( mesh->m_ib->getData(), indices, size );

    // setup primitives
    mesh->createPrimitiveGroup(IndexType::UnsignedShort, 3, PrimitiveType::TriangleList, 0);

    // setup material
    mesh->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    mActiveGeo = mesh;

    return *this;
}

MeshBuilder &MeshBuilder::allocQuads( VertexType type, BufferAccessType access ) {
    Mesh *mesh = Mesh::create(1, type);
    mesh->m_indextype = IndexType::UnsignedShort;

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

    mesh->m_vb = allocVertices( mesh->m_vertextype, NumVert, pos, col, tex0, access );

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
    mesh->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    ::memcpy( mesh->m_ib->getData(), indices, size );

    // setup primitives
    mesh->createPrimitiveGroup(IndexType::UnsignedShort, NumIndices, PrimitiveType::TriangleList, 0);

    // setup material
    mesh->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    mActiveGeo = mesh;

    return *this;
}

MeshBuilder &MeshBuilder::allocUiQuad( const Rect2ui &dim, UiVertexCache &vc, RenderBackend::UiIndexCache &ic ) {
    const f32 x = (f32) dim.getX1();
    const f32 y = (f32) dim.getY1();
    const f32 w = (f32) dim.getWidth();
    const f32 h = (f32) dim.getHeight();

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

    return *this;
}

MeshBuilder &MeshBuilder::allocCube(VertexType type, f32 w, f32 h, f32 d, BufferAccessType access ) {
    mActiveGeo = Mesh::create(1,VertexType::RenderVertex);
    RenderVert v[8];

    //glm::vec3 v[8];
    v[0].position.x = v[0].position.y = v[0].position.z = 0.0f;
    
    v[1].position.x = w;
    v[1].position.y = v[1].position.z = 0.0f;
    
    v[2].position.y = d;
    v[2].position.x = v[2].position.z = 0.0f;
    
    v[3].position.x = w;
    v[3].position.y = d;
    v[3].position.z = 0.0f;

    v[4].position.x = v[4].position.y = 0.0f;
    v[4].position.z = h;
    
    v[5].position.x = w;
    v[5].position.y = 0.0f;
    v[5].position.z = h;
    
    v[6].position.y = d;
    v[6].position.x = 0.0f;
    v[6].position.z = h;
    
    v[7].position.x = w;
    v[7].position.y = d;
    v[7].position.z = h;

    GLushort indices[36] = {
        0,2,1, // bottom
        1,2,3,

        4,5,3,
        3,2,4,

        5,6,2,
        2,1,5,

        6,7,3,
        3,2,6,

        7,6,5,
        5,4,7,

        4,5,6,
        6,7,4
    }; 

    mActiveGeo->m_vertextype = type;
    mActiveGeo->m_indextype = IndexType::UnsignedShort;
    mActiveGeo->m_numPrimGroups = 1;
    mActiveGeo->m_primGroups = new PrimitiveGroup[mActiveGeo->m_numPrimGroups];
    mActiveGeo->m_primGroups[0].init(IndexType::UnsignedShort, 12, PrimitiveType::TriangleList, 0);

    const size_t vbSize = sizeof(RenderVert) * 8;
    mActiveGeo->m_vb = BufferData::alloc(BufferType::VertexBuffer, vbSize, access);
    mActiveGeo->m_vb->copyFrom(&v[0].position.x, vbSize);

    const size_t ibSize = sizeof(GLushort) * 36;
    mActiveGeo->m_ib = BufferData::alloc(BufferType::IndexBuffer, ibSize, BufferAccessType::ReadOnly);
    mActiveGeo->m_ib->copyFrom(indices, ibSize);

    mActiveGeo->m_material = Scene::MaterialBuilder::createBuildinMaterial(type);

    return *this;
}

MeshBuilder &MeshBuilder::allocLineList( VertexType type, BufferAccessType access, ui32 numLines,
                                          glm::vec3 *posArray, glm::vec3 *colorArray, ui32 *indices ) {
    Mesh *geo = Mesh::create(1, type);
    geo->m_indextype = IndexType::UnsignedShort;

    geo->m_numPrimGroups = 1;
    geo->m_primGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_primGroups[ 0 ].init( IndexType::UnsignedShort, 2 * numLines, PrimitiveType::LineList, 0 );

    geo->m_vb = Scene::MeshBuilder::allocVertices( type, numLines, posArray, colorArray, nullptr, access );
    geo->m_indextype = IndexType::UnsignedShort;
    ui32 size = sizeof( GLushort ) * numLines*2;
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    geo->m_ib->copyFrom( indices, size );

    mActiveGeo = geo;

    return *this;
}

MeshBuilder &MeshBuilder::allocPoints( VertexType type, BufferAccessType access, ui32 numPoints,
                                        glm::vec3 *posArray, glm::vec3 *colorArray ) {
    // colors
    CPPCore::TArray<ui16> indices;
    indices.resize( numPoints );
    for ( ui16 i = 0; i < numPoints; i++ ) {
        indices[ i ] = i;
    }

    Mesh *ptGeo = Mesh::create(1, type);

    ptGeo->m_vb = Scene::MeshBuilder::allocVertices(VertexType::ColorVertex, numPoints, posArray, 
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

    mActiveGeo = ptGeo;

    return *this;
}

static const size_t NumQuadVert = 4;

static size_t getNumTextVerts( const String &text ) {
    const size_t NumTextVerts = NumQuadVert * text.size();
    return NumTextVerts;
}

static const ui32 NumQuadIndices = 6;

static size_t getNumTextIndices(const String &text) {
    const size_t numIndices = NumQuadIndices * text.size();
    return numIndices;
}

static void generateTextBoxVerticesAndIndices(f32 x, f32 y, f32 textSize, const String &text, 
        glm::vec3 **textPos, glm::vec3 **colors, glm::vec2 **tex0, GLushort **textIndices) {
    osre_assert(nullptr != textPos);
    osre_assert(nullptr != colors);
    osre_assert(nullptr != tex0);
    osre_assert(nullptr != textIndices);

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

    const size_t NumTextVerts = getNumTextVerts(text);
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
            ++textRow;
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

MeshBuilder &MeshBuilder::allocTextBox( f32 x, f32 y, f32 textSize, const String &text, BufferAccessType access ) {
	if ( text.empty() ) {
		return *this;
	}

    Mesh *mesh = Mesh::create(1, VertexType::RenderVertex);
    mesh->m_indextype = IndexType::UnsignedShort;

    glm::vec3 *textPos(nullptr), *colors(nullptr);
    glm::vec2 *tex0(nullptr);
    GLushort *textIndices(nullptr);
    generateTextBoxVerticesAndIndices(x,y,textSize, text, &textPos, &colors, &tex0, &textIndices);

    //GeometryDiagnosticUtils::dumpIndices( textIndices, 6 * text.size() );

    mesh->m_vb = allocVertices( mesh->m_vertextype, text.size() * NumQuadVert, textPos, colors, tex0, access );

    // setup triangle indices
    size_t size = sizeof( GLushort ) * 6 * text.size();
    mesh->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
    mesh->m_ib->copyFrom(textIndices, size);

    // setup primitives
    mesh->m_numPrimGroups = 1;
    mesh->m_primGroups = new PrimitiveGroup[ 1 ];
    mesh->m_primGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    mesh->m_primGroups[ 0 ].m_numIndices = 6 * text.size();
    mesh->m_primGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    mesh->m_primGroups[ 0 ].m_startIndex = 0;

    // setup material
    CPPCore::TArray<TextureResource*> texResArray;;
    TextureResource* texRes = new TextureResource("buildin_arial", IO::Uri("file://assets/Textures/Fonts/buildin_arial.bmp"));
    texResArray.add(texRes);
    mesh->m_material = Scene::MaterialBuilder::createTexturedMaterial("text_box_tex", texResArray, VertexType::RenderVertex );
    mActiveGeo = mesh;

    return *this;
}

static f32 getZbyStackIndex(f32 stackIndex) {
    const f32 result = stackIndex * -0.1f;
    return result;
}

void MeshBuilder::allocUiTextBox(f32 x, f32 y, i32 stackIndex, f32 textSize, const String &text, BufferAccessType access,
        UiVertexCache &vc, UiIndexCache &ic) {
    glm::vec3 *textPos(nullptr), *colors(nullptr);
    glm::vec2 *tex0(nullptr);
    GLushort *textIndices(nullptr);
    generateTextBoxVerticesAndIndices(x, y, textSize, text, &textPos, &colors, &tex0, &textIndices);
    const size_t offset = vc.numVertices();
    const size_t numNewVerts = getNumTextVerts(text);
    for (size_t i = 0; i < numNewVerts; i++) {

        RenderVert v;
        v.position = textPos[i];
//        UI::WidgetCoordMapping::mapPosToWorld(static_cast<ui32>(v.position.x), static_cast<ui32>(v.position.y), v.position.x, v.position.y);


        v.position.z = (f32) getZbyStackIndex((f32)stackIndex);
        v.color0 = colors[i];
        v.tex0 = tex0[i];
        vc.add(v);
    }

    const size_t numIndices = getNumTextIndices(text);
    for (size_t i = 0; i < numIndices; ++i) {
        ic.add(textIndices[i] + ( GLushort ) offset);
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

    const size_t numTextVerts( getNumTextVerts( text ) );
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
    for ( ui32 i = 0; i < text.size(); i++ ) {
        const c8 ch = text[ i ];
        if ( Tokenizer::isLineBreak( ch ) ) {
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

BufferData *MeshBuilder::allocVertices( VertexType type, size_t numVerts, glm::vec3 *pos,
                                            glm::vec3 *col1, glm::vec2 *tex0, BufferAccessType access ) {
    BufferData *data( nullptr );
    size_t size( 0 );
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

void MeshBuilder::updateTextVertices( size_t numVerts, ::glm::vec2 *tex0, BufferData *vb ) {
    if (0 == numVerts) {
        return;
    }
    osre_assert(nullptr != tex0);
    osre_assert(nullptr != vb);

    RenderVert *vert = new RenderVert[ numVerts ];
    ::memcpy( &vert[ 0 ].position, vb->getData(), vb->getSize() );
    for ( ui32 i = 0; i < numVerts; i++ ) {
        vert[ i ].tex0 = tex0[ i ];
    }
    ::memcpy( vb->getData(), vert, vb->getSize() );
    delete[] vert;
}

RenderBackend::Mesh *MeshBuilder::getMesh() {
    return mActiveGeo;
}

} // Namespace Scene
} // Namespace OSRE
