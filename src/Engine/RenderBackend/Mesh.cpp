/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/Mesh.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Common/Logger.h>
#include <osre/Common/Ids.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

// Id container used for geometries
static Ids s_Ids;

// The log tag for messages
static const c8 *Tag = "Mesh";

Mesh::Mesh()
: m_localMatrix( false )
, m_model( 1.0f )
, m_material( nullptr )
, m_vertextype( VertexType::RenderVertex )
, m_vb( nullptr )
, m_ib( nullptr )
, m_numPrimGroups( 0 )
, m_primGroups( nullptr )
, m_id( 99999999 )
, m_vertexData()
, m_indexData()
, m_lastIndex( 0 ) {
    // empty
}

Mesh::~Mesh() {
    delete m_material;
    m_material = nullptr;

    delete m_vb;
    m_vb = nullptr;

    delete m_ib;
    m_ib = nullptr;

    delete[] m_primGroups;
    m_primGroups = nullptr;

    s_Ids.releaseId( m_id );
}

Mesh *Mesh::create( ui32 numGeo ) {
    if ( 0 == numGeo ) {
        osre_debug( Tag, "Number of static geo to create is zero." );
        return nullptr;
    }
    Mesh *geoArray( new Mesh[ numGeo ] );
    for ( ui32 i = 0; i < numGeo; i++ ) {
        geoArray[ i ].m_id = s_Ids.getUniqueId();
    }
    return geoArray;
}

void Mesh::destroy( Mesh **geo ) {
    delete[] * geo;
    ( *geo ) = nullptr;
}

ui32 Mesh::getVertexSize( VertexType vertextype ) {
    ui32 vertexSize( 0 );
    switch ( vertextype ) {
        case VertexType::ColorVertex:
            vertexSize = sizeof( ColorVert );
            break;

        case VertexType::RenderVertex:
            vertexSize = sizeof( RenderVert );
            break;

        default:
            break;
    }

    return vertexSize;
}

PrimitiveGroup *Mesh::createPrimitiveGroups(size_t numPrimGroups, IndexType* types, size_t* numIndices, PrimitiveType* primTypes, ui32* startIndices) {
    if (0 == numPrimGroups) {
        return nullptr;
    }

    OSRE_ASSERT(nullptr != types);
    OSRE_ASSERT(nullptr != numIndices);
    OSRE_ASSERT(nullptr != primTypes);
    OSRE_ASSERT(nullptr != startIndices);

    m_numPrimGroups = numPrimGroups;
    m_primGroups = new PrimitiveGroup[m_numPrimGroups];
    for (size_t i = 0; i < m_numPrimGroups; ++i) {
        m_primGroups[i].m_indexType = types[i];
        m_primGroups[i].m_numIndices = numIndices[i];
        m_primGroups[i].m_primitive = primTypes[i];
        m_primGroups[i].m_startIndex = startIndices[i];
    }

    return m_primGroups;
}

PrimitiveGroup *Mesh::createPrimitiveGroup(IndexType type, size_t numIndices, PrimitiveType primType, ui32 startIndex) {
    m_numPrimGroups = 1;
    m_primGroups = new PrimitiveGroup[m_numPrimGroups];
    m_primGroups[0].m_indexType = type;
    m_primGroups[0].m_numIndices = numIndices;
    m_primGroups[0].m_primitive = primType;
    m_primGroups[0].m_startIndex = startIndex;

    return m_primGroups;
}

} // Namespace RenderBackend
} // Namespace OSRE
