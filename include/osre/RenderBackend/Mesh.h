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

struct OSRE_EXPORT Mesh {
    bool            m_localMatrix;
    glm::mat4       m_model;
    Material       *m_material;
    VertexType      m_vertextype;
    BufferData     *m_vb;
    IndexType       m_indextype;
    BufferData     *m_ib;
    ui32            m_numPrimGroups;
    PrimitiveGroup *m_primGroups;
    ui32            m_id;

    static Mesh *create( ui32 numGeo );
    static void destroy( Mesh **geo );
    static ui32 getVertexSize( VertexType vertextype );

    template<class TVertexType>
    inline
    static ui32 getVertexSize() {
        return sizeof( TVertexType );
    }

    OSRE_NON_COPYABLE( Mesh )

private:
    Mesh();
    ~Mesh();

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


} // Namespace RenderBackend
} // Namespace OSRE
