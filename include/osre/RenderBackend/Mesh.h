/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

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
    Mesh(const String &name, VertexType vertextype, IndexType indextype);
    ~Mesh();
    static size_t getVertexSize(VertexType vertextype);
    void setMaterial(Material *mat);
    Material *getMaterial() const;
    VertexType getVertexType() const;
    void setIndexType(IndexType indextype);
    IndexType getIndexType() const;
    const String &getName() const;
    void *mapVertexBuffer(size_t vbSize, BufferAccessType accessType);
    void unmapVertexBuffer();
    void createVertexBuffer(void *vertices, size_t vbSize, BufferAccessType accessType);
    BufferData *getVertexBuffer() const;
    void createIndexBuffer(void *indices, size_t ibSize, IndexType indexType, BufferAccessType accessType);
    BufferData *getIndexBuffer() const;
    ui64 getId() const;
    size_t getNumberOfPrimitiveGroups() const;
    PrimitiveGroup *getPrimitiveGroupAt(size_t index) const;
    void setModelMatrix(bool islocal, const glm::mat4 &model);
    bool isLocal() const;
    const glm::mat4 &getLocalMatrix() const;
    template <class T>
    void attachVertices(T *vertices, size_t size) {
        if (mVertexBuffer == nullptr) {
            mVertexBuffer = BufferData::alloc(BufferType::VertexBuffer, size, BufferAccessType::ReadWrite);
            ::memcpy(mVertexBuffer->getData(), vertices, size);
        } else {
            mVertexBuffer->attach(vertices, size);
        }
    }

    template <class T>
    void attachIndices(T *indices, size_t size) {
        if (mIndexBuffer == nullptr) {
            mIndexBuffer = BufferData::alloc(BufferType::IndexBuffer, size, BufferAccessType::ReadWrite);
            ::memcpy(mIndexBuffer->getData(), indices, size);
        } else {
            mVertexBuffer->attach(indices, size);
        }
    }

    PrimitiveGroup *createPrimitiveGroups(size_t numPrimGroups, size_t *numIndices, PrimitiveType *primTypes, ui32 *startIndices);
    PrimitiveGroup *createPrimitiveGroup(size_t numIndices, PrimitiveType primTypes, ui32 startIndex);
    void setPrimitiveGroups(size_t numPrimGroups, PrimitiveGroup *primGroups);
    OSRE_NON_COPYABLE(Mesh)

private:
    String mName;
    bool m_localMatrix;
    glm::mat4 mModel;
    Material *mMaterial;
    VertexType mVertexType;
    BufferData *mVertexBuffer;
    IndexType mIndexType;
    BufferData *mIndexBuffer;
    size_t mNumPrimGroups;
    PrimitiveGroup *mPrimGroups;
    ui64 mId;
    ::CPPCore::TArray<uc8> mVertexData;
    ::CPPCore::TArray<uc8> mIndexData;
    ui32 mLastIndex;
};

inline void Mesh::setMaterial( Material *mat ) {
    mMaterial = mat;
}

inline Material *Mesh::getMaterial() const {
    return mMaterial;
}

inline VertexType Mesh::getVertexType() const {
    return mVertexType;
}

inline void Mesh::setIndexType( IndexType indextype ) {
    mIndexType = indextype;
}

inline IndexType Mesh::getIndexType() const {
    return mIndexType;
}

inline const String &Mesh::getName() const {
    return mName;
}

template <class TVertexType>
inline size_t getVertexTypeSize() {
    return sizeof(TVertexType);
}

inline ui64 Mesh::getId() const {
    return mId;
}

inline size_t Mesh::getNumberOfPrimitiveGroups() const {
    return mNumPrimGroups;
}

inline PrimitiveGroup *Mesh::getPrimitiveGroupAt( size_t index ) const {
    if (index >= mNumPrimGroups) {
        return nullptr;
    }

    return &mPrimGroups[index];
}

inline void Mesh::setModelMatrix( bool islocal, const glm::mat4 &model ) {
    m_localMatrix = islocal;
    mModel = model;
}

inline const glm::mat4 &Mesh::getLocalMatrix() const {
    return mModel;
}

inline bool Mesh::isLocal() const {
    return m_localMatrix;
}

} // Namespace RenderBackend
} // Namespace OSRE
