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

#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

struct OSRE_EXPORT Geometry {
    bool            m_localMatrix;
    glm::mat4       m_model;
    Material       *m_material;
    VertexType      m_vertextype;
    BufferData     *m_vb;
    IndexType       m_indextype;
    BufferData     *m_ib;
    ui32            m_numPrimGroups;
    PrimitiveGroup *m_pPrimGroups;
    ui32            m_id;

    static Geometry *create( ui32 numGeo );
    static void destroy( Geometry **geo );
    static ui32 getVertexSize( VertexType vertextype );

    template<class TVertexType>
    void addLine( const TVertexType &v1, const TVertexType &v2 );
    
    template<class TVertexType>
    void addTriangle( const TVertexType &v0, const TVertexType &v1, const TVertexType &v2 );

    void commitNewGeometry();

    OSRE_NON_COPYABLE( Geometry )

private:
    Geometry();
    ~Geometry();

private:
    ::CPPCore::TArray<uc8> m_vertexData;
    ::CPPCore::TArray<uc8> m_indexData;
    ui32 m_lastIndex;
};

template<class TVertexType>
inline
ui32 getVertexTypeSize() {
    return sizeof( TVertexType );
}

template<class TVertexType>
inline
void Geometry::addLine( const TVertexType &v0, const TVertexType &v1 ) {
    ui32 size = getVertexTypeSize( v0 );
    ui32 offset = m_vertexData.size();
    m_vertexData.resize( m_vertexData.size()*size*2 );
    ::memcpy( m_vertexData[ offset ], &v0, size );
    offset += size;
    ::memcpy( m_vertexData[ offset ], &v1, size );
    offset += size;

    offset = m_indexData.size();
    m_indexData.resize( m_indexData.size() + 2 * sizeof( ui32 ) );
    ::memcpy( m_indexData[ offset ], &m_lastIndex, sizeof( ui32 ) );
    m_indexData++;
    ::memcpy( m_indexData[ offset ], &m_lastIndex, sizeof( ui32 ) );
    m_indexData++;
}

template<class TVertexType>
inline
void Geometry::addTriangle( const TVertexType &v0, const TVertexType &v1, const TVertexType &v2 ) {
    ui32 size = getVertexTypeSize( v0 );
    ui32 offset = m_vertexData.size();
    m_vertexData.resize( m_vertexData.size()*size * 2 );
    ::memcpy( m_vertexData[ offset ], &v0, size );
    offset += size;
    ::memcpy( m_vertexData[ offset ], &v1, size );
    offset += size;
    ::memcpy( m_vertexData[ offset ], &v2, size );
    offset += size;

    offset = m_indexData.size();
    m_indexData.resize( m_indexData.size() + 2 * sizeof( ui32 ) );
    ::memcpy( m_indexData[ offset ], &m_lastIndex, sizeof( ui32 ) );
    m_indexData++;
    ::memcpy( m_indexData[ offset ], &m_lastIndex, sizeof( ui32 ) );
    m_indexData++;
    ::memcpy( m_indexData[ offset ], &m_lastIndex, sizeof( ui32 ) );
    m_indexData++;
}

} // Namespace RenderBackend
} // Namespace OSRE

