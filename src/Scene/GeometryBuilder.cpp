/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

namespace intern {
    static void dumpTextBox( ui32 i, glm::vec3 *textPos, ui32 VertexOffset ) {
        std::stringstream stream;
        stream << std::endl;
        stream << "i = " << i << " : " << textPos[ VertexOffset + 0 ].x << ", " << textPos[ VertexOffset + 0 ].y << std::endl;
        stream << "i = " << i << " : " << textPos[ VertexOffset + 1 ].x << ", " << textPos[ VertexOffset + 1 ].y << std::endl;
        stream << "i = " << i << " : " << textPos[ VertexOffset + 2 ].x << ", " << textPos[ VertexOffset + 2 ].y << std::endl;
        stream << "i = " << i << " : " << textPos[ VertexOffset + 3 ].x << ", " << textPos[ VertexOffset + 3 ].y << std::endl;
        osre_info( Tag, stream.str() );
    }

    static void dumpTextTex0Box( ui32 i, glm::vec2 *tex0Pos, ui32 VertexOffset ) {
        std::stringstream stream;
        stream << std::endl;
        stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 0 ].x << ", " << tex0Pos[ VertexOffset + 0 ].y << std::endl;
        stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 1 ].x << ", " << tex0Pos[ VertexOffset + 1 ].y << std::endl;
        stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 2 ].x << ", " << tex0Pos[ VertexOffset + 2 ].y << std::endl;
        stream << "i = " << i << " : " << tex0Pos[ VertexOffset + 3 ].x << ", " << tex0Pos[ VertexOffset + 3 ].y << std::endl;
        osre_info( Tag, stream.str() );
    }
}

GeometryBuilder::GeometryBuilder() {
    // empty
}

GeometryBuilder::~GeometryBuilder() {
    // empty
}

StaticGeometry *GeometryBuilder::allocEmptyGeometry( VertexType type, ui32 numGeo ) {
    StaticGeometry *geo = StaticGeometry::create( numGeo );
    for ( ui32 i = 0; i < numGeo; i++ ) {
        geo[ i ].m_vertextype = type;
        geo[ i ].m_indextype = UnsignedShort;

    }

    return geo;
}

StaticGeometry *GeometryBuilder::allocTriangles( VertexType type ) {
    StaticGeometry *geo = StaticGeometry::create( 1 );
    geo->m_vertextype = type;
    geo->m_indextype = UnsignedShort;

    // setup triangle vertices    
    static const ui32 NumVert = 3;
    ColorVert vertices[ NumVert ];

    glm::vec3 col[ NumVert ];
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );

    glm::vec3 pos[ NumVert ];
    pos[ 0 ] = glm::vec3( -1, -1, 0 );
    pos[ 1 ] = glm::vec3( 0, 1, 0 );
    pos[ 2 ] = glm::vec3( 1, -1, 0 );
    geo->m_vb = allocVertices( geo->m_vertextype,  NumVert, pos, col, nullptr );

    // setup triangle indices
    static const ui32 NumIndices = 3;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;
    
    ui32 size = sizeof( GLushort ) * NumIndices;
    geo->m_ib = BufferData::alloc( IndexBuffer, size, ReadOnly );
    ::memcpy( geo->m_ib->m_data, indices, size );

	// setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups   = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups[ 0 ].m_indexType     = UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 3 * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive     = TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex    = 0;

	// setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    return geo;
}

StaticGeometry *GeometryBuilder::allocQuads( VertexType type ) {
    StaticGeometry *geo = StaticGeometry::create( 1 );
    geo->m_vertextype = type;
    geo->m_indextype = UnsignedShort;

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

    geo->m_vb = allocVertices( geo->m_vertextype, NumVert, pos, col, nullptr );

    // setup triangle indices
    static const ui32 NumIndices = 6;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;

    indices[ 3 ] = 1;
    indices[ 4 ] = 2;
    indices[ 5 ] = 3;

    ui32 size = sizeof( GLushort ) * NumIndices;
    geo->m_ib = BufferData::alloc( IndexBuffer, size, ReadOnly );
    ::memcpy( geo->m_ib->m_data, indices, size );

    // setup primitives
    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups[ 0 ].m_indexType = UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 6 * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive = TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    // setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( type );

    return geo;
}

static bool isLineBreak(c8 c) {
    if (c == '\n') {
        return true;
    } else {
        return false;
    }
}

StaticGeometry *GeometryBuilder::allocTextBox( f32 x, f32 y, f32 textSize, const String &text ) {
	if ( text.empty() ) {
		return nullptr;
	}

    StaticGeometry *geo = StaticGeometry::create( 1 );
    geo->m_vertextype = RenderVertex;
    geo->m_indextype = UnsignedShort;

    // setup triangle vertices    
    static const ui32 NumQuadVert = 4;
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

    indices[ 3 ] = 1;
    indices[ 4 ] = 2;
    indices[ 5 ] = 3;

    const ui32 NumTextVerts = NumQuadVert * text.size();
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
        
        const ui32 VertexOffset( i * NumQuadVert );
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
		tex0[VertexOffset + 0].y = 1.0 - t;

		tex0[VertexOffset + 1].x = s;
        tex0[VertexOffset + 1].y = 1.0 - t + 1.0f / 16.0f;

		tex0[VertexOffset + 2].x = s + 1.0f/16.0f;
        tex0[VertexOffset + 2].y = 1.0 - t;
       
        tex0[VertexOffset + 3].x = s + 1.0f / 16.0f;
        tex0[VertexOffset + 3].y = 1.0 - t + 1.0f / 16.0f;
        
        //dumpTextTex0Box(i, tex0, VertexOffset);
        colors[ VertexOffset + 0 ] = col[ 0 ];
        colors[ VertexOffset + 1 ] = col[ 1 ];
        colors[ VertexOffset + 2 ] = col[ 2 ];
        colors[ VertexOffset + 3 ] = col[ 3 ];
        const ui32 IndexOffset( i * NumQuadIndices );
        textIndices[ 0 + IndexOffset ] = 0 + VertexOffset;
        textIndices[ 1 + IndexOffset ] = 1 + VertexOffset;
        textIndices[ 2 + IndexOffset ] = 2 + VertexOffset;

        textIndices[ 3 + IndexOffset ] = 1 + VertexOffset;
        textIndices[ 4 + IndexOffset ] = 2 + VertexOffset;
        textIndices[ 5 + IndexOffset ] = 3 + VertexOffset;
        textCol++;
    }

    geo->m_vb = allocVertices( geo->m_vertextype, text.size() * NumQuadVert, textPos, colors, tex0 );

    // setup triangle indices
    ui32 size = sizeof( GLushort ) * 6 * text.size();
    geo->m_ib = BufferData::alloc( IndexBuffer, size, ReadOnly );
    ::memcpy( geo->m_ib->m_data, textIndices, size );

    // setup primitives
    geo->m_numPrimGroups = text.size();
//    geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups = new PrimitiveGroup[ 1 ];
    geo->m_pPrimGroups[ 0 ].m_indexType = UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 6 * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive = TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    // setup material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( RenderVertex );

    // setup the texture
    geo->m_material->m_numTextures = 1;
    geo->m_material->m_pTextures = new Texture[ 1 ];    
    geo->m_material->m_pTextures[ 0 ].m_textureName = "buildin_arial";
    geo->m_material->m_pTextures[ 0 ].m_loc = IO::Uri( "file://assets/Textures/Fonts/buildin_arial.bmp" );

    geo->m_material->m_pTextures[0].m_targetType = Texture2D;
    geo->m_material->m_pTextures[0].m_width = 0;
    geo->m_material->m_pTextures[0].m_height = 0;
    geo->m_material->m_pTextures[0].m_channels = 0;
    geo->m_material->m_pTextures[0].m_data = nullptr;
    geo->m_material->m_pTextures[0].m_size = 0;

    return geo;
}

BufferData *GeometryBuilder::allocVertices( VertexType type, ui32 numVerts, glm::vec3 *pos, 
                                            glm::vec3 *col1, glm::vec2 *tex0 ) {
    BufferData *data( nullptr );
    ui32 size( 0 );
    switch (type) {
        case ColorVertex: {
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
            data = BufferData::alloc( VertexBuffer, size, ReadOnly );
            ::memcpy( data->m_data, colVerts, size );
            delete [] colVerts;
        }
        break;

        case RenderVertex: {
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
            data = BufferData::alloc( VertexBuffer, size, ReadOnly );
            ::memcpy( data->m_data, renderVerts, size );
            delete [] renderVerts;
        }
        break;

        default:
            break;
    }

    return data;
}

} // Namespace Scene
} // namespace OSRE
