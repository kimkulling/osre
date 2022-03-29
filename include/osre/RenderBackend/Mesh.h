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

    i32 mParent;
    String m_name;
    VertexWeightArray m_vertexWeights;
    glm::mat4 m_offsetMatrix;

    Bone() :
            mParent(-1),    
            m_name(),
            m_vertexWeights(),
            m_offsetMatrix() {
        // empty
    }
};

struct OSRE_EXPORT Skeleton {
    using BoneArray = CPPCore::TArray<Bone*>;

    String mName;
    i32 mRootBone;
    BoneArray mBones;

    Skeleton() {}
};

class OSRE_EXPORT Mesh {
public:
    Mesh(const String &name, VertexType vertextype);
    ~Mesh();
    static size_t getVertexSize(VertexType vertextype);
    void setMaterial(Material *mat);
    Material *getMaterial() const;
    VertexType getVertexcType() const;
    const String &getName() const;
    void createVertexBuffer(void *vertices, size_t vbSize, BufferAccessType accessType);
    BufferData *getVertexBuffer() const;
    void createIndexBuffer(void *indices, size_t ibSize, IndexType indexType, BufferAccessType accessType);
    BufferData *getIndexBuffer() const;
    ui64 getId() const;
    size_t getNumberOfPrimitiveGroups() const;
    PrimitiveGroup *getPrimitiveGroupAt(size_t index) const;
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
    String mName;
    bool m_localMatrix;
    glm::mat4 m_model;
    Material *m_material;
    VertexType m_vertextype;
    BufferData *m_vb;
    IndexType m_indextype;
    BufferData *m_ib;
    size_t m_numPrimGroups;
    PrimitiveGroup *m_primGroups;
    ui64 m_id;

    ::CPPCore::TArray<uc8> mVertexData;
    ::CPPCore::TArray<uc8> mIndexData;
    ui32 m_lastIndex;
};

inline void Mesh::setMaterial( Material *mat ) {
    m_material = mat;
}

inline Material *Mesh::getMaterial() const {
    return m_material;
}

inline VertexType Mesh::getVertexcType() const {
    return m_vertextype;
}

inline const String &Mesh::getName() const {
    return mName;
}

template <class TVertexType>
inline size_t getVertexTypeSize() {
    return sizeof(TVertexType);
}

inline ui64 Mesh::getId() const {
    return m_id;
}

inline size_t Mesh::getNumberOfPrimitiveGroups() const {
    return m_numPrimGroups;
}

inline PrimitiveGroup *Mesh::getPrimitiveGroupAt( size_t index ) const {
    if (index >= m_numPrimGroups) {
        return nullptr;
    }

    return &m_primGroups[index];
}

} // Namespace RenderBackend
} // Namespace OSRE
