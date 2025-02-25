/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Common/glm_common.h"
#include "RenderBackend/RenderCommon.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

// Forward declarations ---------------------------------------------------------------------------
class Material;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Mesh {
public:
    Mesh(const String &name, VertexType vertextype, IndexType indextype);
    ~Mesh();
    static size_t getVertexSize(VertexType vertextype);
    void setMaterial(Material *mat);
    Material *getMaterial() const;
    VertexType getVertexType() const;
    IndexType getIndexType() const;
    const String &getName() const;
    void *mapVertexBuffer(size_t vbSize, BufferAccessType accessType);
    void unmapVertexBuffer();
    void createVertexBuffer(void *vertices, size_t vbSize, BufferAccessType accessType);
    void resizeVertexBuffer(size_t vbSize);
    BufferData *getVertexBuffer() const;
    void createIndexBuffer(void *indices, size_t ibSize, IndexType indexType, BufferAccessType accessType);
    BufferData *getIndexBuffer() const;
    void setId(guid id);
    guid getId() const;
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
            mIndexBuffer->attach(indices, size);
        }
    }

    void addPrimitiveGroups(size_t numPrimGroups, size_t *numIndices, PrimitiveType *primTypes, ui32 *startIndices);
    void addPrimitiveGroup(size_t numIndices, PrimitiveType primTypes, ui32 startIndex);
    void addPrimitiveGroup(PrimitiveGroup *group);
    void setLastIndex(ui32 lastIndex);
    ui32 getLastIndex() const;

    OSRE_NON_COPYABLE(Mesh)

private:
    using PrimGroupArray = ::cppcore::TArray<PrimitiveGroup*>;

    String mName;
    bool mLocalModelMatrix;
    glm::mat4 mModel;
    Material *mMaterial;
    VertexType mVertexType;
    BufferData *mVertexBuffer;
    IndexType mIndexType;
    BufferData *mIndexBuffer;
    PrimGroupArray mPrimGroups;
    guid mId;
    MemoryBuffer mVertexData;
    MemoryBuffer mIndexData;
    ui32 mLastIndex;
};

inline void Mesh::setMaterial(Material *mat) {
    mMaterial = mat;
}

inline Material *Mesh::getMaterial() const {
    return mMaterial;
}

inline VertexType Mesh::getVertexType() const {
    return mVertexType;
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

inline void Mesh::setId(guid id) {
    mId = id;
}

inline guid Mesh::getId() const {
    return mId;
}

inline size_t Mesh::getNumberOfPrimitiveGroups() const {
    return mPrimGroups.size();
}

inline PrimitiveGroup *Mesh::getPrimitiveGroupAt( size_t index ) const {
    if (index >= mPrimGroups.size()) {
        return nullptr;
    }

    return mPrimGroups[index];
}

inline void Mesh::setModelMatrix( bool islocal, const glm::mat4 &model ) {
    mLocalModelMatrix = islocal;
    mModel = model;
}

inline const glm::mat4 &Mesh::getLocalMatrix() const {
    return mModel;
}

inline bool Mesh::isLocal() const {
    return mLocalModelMatrix;
}

inline void Mesh::setLastIndex(ui32 lastIndex) {
    mLastIndex = lastIndex;
}

inline ui32 Mesh::getLastIndex() const {
    return mLastIndex;
}

} // Namespace RenderBackend
} // Namespace OSRE
