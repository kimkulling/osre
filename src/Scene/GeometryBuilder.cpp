#include <osre/Scene/GeometryBuilder.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
GeometryBuilder::GeometryBuilder() {
    // empty
}

//-------------------------------------------------------------------------------------------------
GeometryBuilder::~GeometryBuilder() {
    // empty
}

//-------------------------------------------------------------------------------------------------
RenderBackend::Geometry *GeometryBuilder::createTriangle() {
    Geometry *pGeometry = new Geometry;
    pGeometry->m_vertextype = ColorVertex;
    pGeometry->m_indextype = UnsignedShort;

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
    pGeometry->m_pVertexBuffer = BufferData::alloc( VertexBuffer, size, ReadOnly );
    ::memcpy( pGeometry->m_pVertexBuffer->m_pData, vertices, size );

    // setup triangle indices
    static const ui32 NumIndices = 3;
    GLushort  indices[ NumIndices ];
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;
    
    size = sizeof( GLushort ) * NumIndices;
    pGeometry->m_pIndexBuffer = BufferData::alloc( IndexBuffer, size, ReadOnly );
    ::memcpy( pGeometry->m_pIndexBuffer->m_pData, indices, size );

    pGeometry->m_numPrimGroups = 1;
    pGeometry->m_pPrimGroups   = new PrimitiveGroup[ pGeometry->m_numPrimGroups ];
    pGeometry->m_pPrimGroups[ 0 ].m_indexType     = UnsignedShort;
    pGeometry->m_pPrimGroups[ 0 ].m_numPrimitives = 3 * pGeometry->m_numPrimGroups;
    pGeometry->m_pPrimGroups[ 0 ].m_primitive     = TriangleList;
    pGeometry->m_pPrimGroups[ 0 ].m_startIndex    = 0;

    return pGeometry;
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
    pGeometry->m_pVertexBuffer = BufferData::alloc( VertexBuffer, size, ReadOnly );
    ::memcpy( pGeometry->m_pVertexBuffer->m_pData, vertices, size );

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
    pGeometry->m_pIndexBuffer = BufferData::alloc( IndexBuffer, size, ReadOnly );
    ::memcpy( pGeometry->m_pIndexBuffer->m_pData, indices, size );

    return pGeometry;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Scene
} // namespace OSRE
