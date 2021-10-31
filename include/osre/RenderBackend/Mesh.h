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
#pragma once

#include <osre/Common/glm_common.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

struct OSRE_EXPORT VertexWeight {
    ui32 m_vertexIdx;
    f32 m_vertexWeight;

    bool operator == ( const VertexWeight &rhs ) {
        return (m_vertexIdx == rhs.m_vertexIdx && m_vertexWeight == rhs.m_vertexWeight);
    }

    bool operator != (const VertexWeight &rhs) {
        return !(*this == rhs);
    }
};

struct OSRE_EXPORT Bone {
    using VertexWeightArray = CPPCore::TArray<VertexWeight>;

    String m_name;
    VertexWeightArray m_vertexWeights;
    glm::mat4 m_offsetMatrix;

    Bone() :
            m_name(),
            m_vertexWeights(),
            m_offsetMatrix() {
        // empty
    }
};

class OSRE_EXPORT Mesh {
public:
    String m_name;
    Material *m_material;
    VertexType m_vertextype;
    BufferData *m_vb;
    IndexType m_indextype;
    BufferData *m_ib;
    size_t m_numPrimGroups;
    PrimitiveGroup *m_primGroups;
    ui64 m_id;

    static Mesh *create(size_t numGeo, VertexType type);
    static void destroy(Mesh **geo);
    static size_t getVertexSize(VertexType vertextype);

    template <class T>
    void attachVertices(T *vertices, size_t size) {
        if (m_vb == nullptr) {
            m_vb = BufferData::alloc(BufferType::VertexBuffer, size, BufferAccessType::ReadWrite);
            ::memcpy(m_vb->getData(), vertices, size);
        } else {
            m_vb->attach(vertices, size);
        }
    }

    template <class T>
    void attachIndices(T *indices, size_t size) {
        if (m_ib == nullptr) {
            m_ib = BufferData::alloc(BufferType::IndexBuffer, size, BufferAccessType::ReadWrite);
            ::memcpy(m_ib->getData(), indices, size);
        } else {
            m_vb->attach(indices, size);
        }
    }

    PrimitiveGroup *createPrimitiveGroups(size_t numPrimGroups, IndexType *types, size_t *numIndices, PrimitiveType *primTypes, ui32 *startIndices);
    PrimitiveGroup *createPrimitiveGroup(IndexType type, size_t numIndices, PrimitiveType primTypes, ui32 startIndex);

    OSRE_NON_COPYABLE(Mesh)

private:
    Mesh();
    Mesh(VertexType type);
    ~Mesh();

private:
    ::CPPCore::TArray<uc8> m_vertexData;
    ::CPPCore::TArray<uc8> m_indexData;
    ui32 m_lastIndex;
};

template <class TVertexType>
inline size_t getVertexTypeSize() {
    return sizeof(TVertexType);
}

} // Namespace RenderBackend
} // Namespace OSRE
