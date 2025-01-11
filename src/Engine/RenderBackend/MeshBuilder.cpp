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
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/Mesh/MeshUtilities.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/FontService.h"
#include "RenderBackend/Mesh.h"
#include "Common/Logger.h"
#include "Common/Tokenizer.h"
#include "Debugging/osre_debugging.h"

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

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

MeshBuilder::MeshBuilder() :
        mIsDirty( false ), 
        mActiveMesh( nullptr ) {
    // empty
}

MeshBuilder::~MeshBuilder() {
    clear();
}

void MeshBuilder::clear() {
    if (mActiveMesh != nullptr) {
        mActiveMesh = nullptr;
    }
}

MeshBuilder &MeshBuilder::allocEmptyMesh(const String &name, VertexType type) {
    clear();
    mActiveMesh = new Mesh(name, type, IndexType::UnsignedShort);

    return *this;
}

MeshBuilder &MeshBuilder::createTriangle(VertexType type, BufferAccessType access) {
    clear();
    mActiveMesh = new Mesh("", type, IndexType::UnsignedShort);

    // setup triangle vertices    
    static const ui32 NumVert = 3;
    glm::vec3 col[NumVert] = {};
    col[ 0 ] = glm::vec3(1, 0, 0);
    col[ 1 ] = glm::vec3(0, 1, 0);
    col[ 2 ] = glm::vec3(0, 0, 1);

    glm::vec3 pos[NumVert] = {};
    pos[ 0 ] = glm::vec3(-1, -1, 0);
    pos[ 1 ] = glm::vec3(0, 1, 0);
    pos[ 2 ] = glm::vec3(1, -1, 0);
    
    allocVertices(mActiveMesh, mActiveMesh->getVertexType(), NumVert, pos, col, nullptr, access);

    // setup triangle indices
    static const size_t NumIndices = 3;
    ui16 indices[NumIndices] = {};
    indices[ 0 ] = 0;
    indices[ 1 ] = 2;
    indices[ 2 ] = 1;

    size_t size = sizeof(ui16) * NumIndices;
    mActiveMesh->createIndexBuffer(indices, size, IndexType::UnsignedShort, access);

    // setup primitives
    mActiveMesh->addPrimitiveGroup(NumIndices, PrimitiveType::TriangleList, 0);

    // setup material
    mActiveMesh->setMaterial(MaterialBuilder::createBuildinMaterial(type));

    return *this;
}

MeshBuilder &MeshBuilder::allocQuads( VertexType type, BufferAccessType access ) {
    clear();
    mActiveMesh = new Mesh("", type, IndexType::UnsignedShort);

    // setup triangle vertices    
    static const ui32 NumVert = 4;
    glm::vec3 col[NumVert] = {};
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );
    col[ 3 ] = glm::vec3( 1, 0, 0 );

    glm::vec3 pos[NumVert] = {};
    pos[ 0 ] = glm::vec3( -1, -1, 0 );
    pos[ 1 ] = glm::vec3( -1, 1, 0 );
    pos[ 2 ] = glm::vec3( 1, -1, 0 );
    pos[ 3 ] = glm::vec3( 1, 1, 0 );

    glm::vec2 tex0[NumVert] = {};
    tex0[ 0 ] = glm::vec2( 0, 0 );
    tex0[ 1 ] = glm::vec2( 0, 1 );
    tex0[ 2 ] = glm::vec2( 1, 0 );
    tex0[ 3 ] = glm::vec2( 1, 1 );

    allocVertices(mActiveMesh, mActiveMesh->getVertexType(), NumVert, pos, col, tex0, access);

    // setup triangle indices
    static const ui32 NumIndices = 6;
    ui16 indices[NumIndices] = {};
    indices[ 0 ] = 0;
    indices[ 1 ] = 2;
    indices[ 2 ] = 1;

    indices[ 3 ] = 1;
    indices[ 4 ] = 2;
    indices[ 5 ] = 3;

    size_t size = sizeof(ui16) * NumIndices;
    mActiveMesh->createIndexBuffer(indices, size, IndexType::UnsignedShort, access);

    // setup primitives
    mActiveMesh->addPrimitiveGroup(NumIndices, PrimitiveType::TriangleList, 0);

    // setup material
    mActiveMesh->setMaterial(MaterialBuilder::createBuildinMaterial(type));

    return *this;
}

MeshBuilder &MeshBuilder::createCube(VertexType type, f32 w, f32 h, f32 d, BufferAccessType access ) {
    clear();
    mActiveMesh = new Mesh("cube", type, IndexType::UnsignedShort);

    static constexpr size_t Numvertices = 8;
    glm::vec3 pos[Numvertices] = {};
    pos[0] = glm::vec3(0, 0, h);
    pos[1] = glm::vec3(w, 0, h);
    pos[2] = glm::vec3(0, d, h);
    pos[3] = glm::vec3(w, d, h);
    
    pos[4] = glm::vec3(0, 0, 0);
    pos[5] = glm::vec3(w, 0, 0);
    pos[6] = glm::vec3(0, d, 0);
    pos[7] = glm::vec3(w, d, 0);

    glm::vec3 col[Numvertices] = {};
    col[0] = glm::vec3(0.5, 0.5, 0.5);
    col[1] = glm::vec3(0.5, 0.5, 0.5);
    col[2] = glm::vec3(0.5, 0.5, 0.5);
    col[3] = glm::vec3(0.5, 0.5, 0.5);
    col[4] = glm::vec3(0.5, 0.5, 0.5);
    col[5] = glm::vec3(0.5, 0.5, 0.5);
    col[6] = glm::vec3(0.5, 0.5, 0.5);
    col[7] = glm::vec3(0.5, 0.5, 0.5);

    allocVertices(mActiveMesh, mActiveMesh->getVertexType(), Numvertices, pos, col, nullptr, access);

    static constexpr size_t NumIndices = 36;
    ui16 indices[NumIndices] = {
        // Top
        2, 6, 7,
        2, 7, 3,

        // Bottom
        0, 4, 5,
        0, 5, 1,

        // Left
        0, 2, 6,
        0, 6, 4,

        // Right
        1, 3, 7,
        1, 7, 5,

        // Front
        0, 3, 2,
        0, 1, 3,

        // Back
        4, 6, 7,
        4, 7, 5
    }; 

    mActiveMesh->addPrimitiveGroup(NumIndices, PrimitiveType::TriangleList, 0);
    
    const size_t ibSize = sizeof(ui16) * NumIndices;
    mActiveMesh->createIndexBuffer(indices, ibSize, IndexType::UnsignedShort, access);
    mActiveMesh->setMaterial(MaterialBuilder::createBuildinMaterial(type));

    return *this;
}

MeshBuilder &MeshBuilder::allocLineList( VertexType type, BufferAccessType access, ui32 numLines,
                                          glm::vec3 *posArray, glm::vec3 *colorArray, ui32 *indices ) {
    clear();
    mActiveMesh = new Mesh("", type, IndexType::UnsignedShort);
    mActiveMesh->addPrimitiveGroup(2 * numLines, PrimitiveType::LineList, 0);

    MeshBuilder::allocVertices(mActiveMesh, type, numLines, posArray, colorArray, nullptr, access);
    ui32 size = sizeof(ui16) * numLines * 2;
    mActiveMesh->createIndexBuffer(indices, size, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

    return *this;
}

MeshBuilder &MeshBuilder::allocPoints( VertexType type, BufferAccessType access, ui32 numPoints,
                                        glm::vec3 *posArray, glm::vec3 *colorArray ) {
    clear();
    // colors
    cppcore::TArray<ui16> indices;
    indices.resize( numPoints );
    for ( ui16 i = 0; i < numPoints; i++ ) {
        indices[ i ] = i;
    }

    mActiveMesh = new Mesh("", type, IndexType::UnsignedShort);

    MeshBuilder::allocVertices(mActiveMesh, VertexType::ColorVertex, numPoints, posArray, 
                        colorArray, nullptr, access );
    const ui32 pt_size = sizeof(ui16) * numPoints;
    mActiveMesh->createIndexBuffer(&indices[0], pt_size, IndexType::UnsignedShort, access);

    // setup primitives
    mActiveMesh->addPrimitiveGroup(3, PrimitiveType::PointList, 0);

    // setup material
    mActiveMesh->setMaterial(MaterialBuilder::createBuildinMaterial(type));

    return *this;
}


MeshBuilder &MeshBuilder::allocTextBox(f32 x, f32 y, f32 textSize, const String &text, BufferAccessType access) {
	if ( text.empty() ) {
		return *this;
	}

    Mesh *mesh = new Mesh("", VertexType::RenderVertex, IndexType::UnsignedShort);

    Vec3Array positions;
    Vec3Array colors;
    Vec2Array tex0;
    ui16 *textIndices(nullptr);
    MeshUtilities::generateTextBoxVerticesAndIndices(x, y, textSize, text, positions, colors, tex0, &textIndices);

    //GeometryDiagnosticUtils::dumpIndices( textIndices, 6 * text.size() );

    allocVertices(mesh, mesh->getVertexType(), text.size() * MeshUtilities::NumQuadVert, &positions[0], &colors[0], &tex0[0], access);

    // setup triangle indices
    size_t size = sizeof(ui16) * 6 * text.size();
    mesh->createIndexBuffer(textIndices, size, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

    // setup primitives
    mesh->addPrimitiveGroup(6 * text.size(), PrimitiveType::TriangleList, 0);

    // setup material
    Font defaultFont;
    defaultFont.Name = "buildin_arial";
    defaultFont.Size = 10;
    defaultFont.Style = 0;
    Material *mat = FontService::getFont(defaultFont);
    /* cppcore::TArray<TextureResource *> texResArray;
    TextureResource* texRes = new TextureResource("buildin_arial", IO::Uri("file://assets/Textures/Fonts/buildin_arial.bmp"));
    texResArray.add(texRes);
    mesh->setMaterial(MaterialBuilder::createTexturedMaterial("text_box_tex", texResArray, VertexType::RenderVertex));*/
    mesh->setMaterial(mat);
    mActiveMesh = mesh;

    return *this;
}

void MeshBuilder::updateTextBox( Mesh *geo, f32 textSize, const String &text ) {
    if ( nullptr == geo ) {
        osre_debug( Tag, "Pointer to geometry is nullptr." );
        return;
    }

    const size_t numTextVerts( MeshUtilities::getNumTextVerts( text ) );
    glm::vec2 *tex0 = new glm::vec2[ numTextVerts ];

    // setup triangle vertices    
    glm::vec3 col[ MeshUtilities::NumQuadVert] = {};
    col[ 0 ] = glm::vec3( 1, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 1, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 1 );
    col[ 3 ] = glm::vec3( 1, 0, 0 );

    glm::vec3 pos[MeshUtilities::NumQuadVert] = {};
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

        const ui32 VertexOffset( i * MeshUtilities::NumQuadVert );

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

    updateTextVertices(  numTextVerts, tex0, geo->getVertexBuffer() );

    delete[] tex0;
}

void MeshBuilder::allocVertices(Mesh *mesh, VertexType type, size_t numVerts, glm::vec3 *pos,
                                            glm::vec3 *col1, glm::vec2 *tex0, BufferAccessType access ) {
    size_t size = 0;
    switch (type) {
        case VertexType::ColorVertex: {
            ColorVert *colVerts = new ColorVert[numVerts];
            if (nullptr != pos) {
                for (ui32 i = 0; i < numVerts; ++i) {
                    colVerts[ i ].position = pos[i];
                }
            }
            if (nullptr != col1 ) {
                for (ui32 i = 0; i < numVerts; ++i) {
                    colVerts[ i ].color0 = col1[i];
                }
            }
            size = sizeof(ColorVert) * numVerts;
            mesh->createVertexBuffer(&colVerts[0], size, access);
            delete [] colVerts;
        }
        break;

        case VertexType::RenderVertex: {
            RenderVert *renderVerts = new RenderVert[numVerts];
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

            size = sizeof(RenderVert) * numVerts;
            mesh->createVertexBuffer(&renderVerts[0], size, access);
            delete [] renderVerts;
        }
        break;

        default:
            break;
    }
}

void MeshBuilder::updateTextVertices(size_t numVerts, ::glm::vec2 *tex0, BufferData *vb) {
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
    return mActiveMesh;
}

} // Namespace RenderBackend
} // Namespace OSRE
