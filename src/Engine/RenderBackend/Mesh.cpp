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
#include <osre/Common/Ids.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/RenderBackend/Mesh.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

// Id container used for geometries
static Ids s_Ids;

// The log tag for messages
static const c8 *Tag = "Mesh";

Mesh::Mesh(const String &name, VertexType vertexType, IndexType indextype) :
        mName(name),
        m_localMatrix(false),
        mModel(1.0f),
        mMaterial(nullptr),
        mVertexType(vertexType),
        mVertexBuffer(nullptr),
        mIndexType(indextype),
        mIndexBuffer(nullptr),
        mNumPrimGroups(0),
        mPrimGroups(nullptr),
        mId(99999999),
        mVertexData(),
        mIndexData(),
        mLastIndex(0) {
    mId = s_Ids.getUniqueId();
}

Mesh::~Mesh() {
    mMaterial = nullptr;

    BufferData::free(mVertexBuffer);
    mVertexBuffer = nullptr;

    BufferData::free(mIndexBuffer);
    mIndexBuffer = nullptr;

    delete[] mPrimGroups;
    mPrimGroups = nullptr;

    s_Ids.releaseId(mId);
}

void *Mesh::mapVertexBuffer( size_t vbSize, BufferAccessType accessType ) {
    mVertexBuffer = BufferData::alloc(BufferType::VertexBuffer, vbSize, accessType);
    return mVertexBuffer->getData();
}

void Mesh::unmapVertexBuffer() {
    // empty
}

void Mesh::createVertexBuffer(void *vertices, size_t vbSize, BufferAccessType accessType) {
    mVertexBuffer = BufferData::alloc(BufferType::VertexBuffer, vbSize, accessType);
    mVertexBuffer->copyFrom(vertices, vbSize);
}

BufferData *Mesh::getVertexBuffer() const {
    return mVertexBuffer;
}

void Mesh::createIndexBuffer(void *indices, size_t ibSize, IndexType indexType, BufferAccessType accessType) {
    mIndexBuffer = BufferData::alloc(BufferType::IndexBuffer, ibSize, accessType);
    mIndexType = indexType;
    mIndexBuffer->copyFrom(indices, ibSize);
}

BufferData *Mesh::getIndexBuffer() const {
    return mIndexBuffer;
}

size_t Mesh::getVertexSize(VertexType vertextype) {
    size_t vertexSize = 0;
    switch (vertextype) {
        case VertexType::ColorVertex:
            vertexSize = sizeof(ColorVert);
            break;

        case VertexType::RenderVertex:
            vertexSize = sizeof(RenderVert);
            break;

        default:
            break;
    }

    return vertexSize;
}

PrimitiveGroup *Mesh::createPrimitiveGroups(size_t numPrimGroups, size_t *numIndices, PrimitiveType *primTypes, ui32 *startIndices) {
    if (0 == numPrimGroups || nullptr == numIndices || nullptr == primTypes || nullptr == startIndices) {
        return nullptr;
    }

    mNumPrimGroups = numPrimGroups;
    mPrimGroups = new PrimitiveGroup[mNumPrimGroups];
    for (size_t i = 0; i < mNumPrimGroups; ++i) {
        mPrimGroups[i].m_indexType = mIndexType;
        mPrimGroups[i].m_numIndices = numIndices[i];
        mPrimGroups[i].m_primitive = primTypes[i];
        mPrimGroups[i].m_startIndex = startIndices[i];
    }

    return mPrimGroups;
}

PrimitiveGroup *Mesh::createPrimitiveGroup(size_t numIndices, PrimitiveType primType, ui32 startIndex) {
    mNumPrimGroups = 1;
    mPrimGroups = new PrimitiveGroup[mNumPrimGroups];
    mPrimGroups[0].m_numIndices = numIndices;
    mPrimGroups[0].m_indexType = mIndexType;
    mPrimGroups[0].m_primitive = primType;
    mPrimGroups[0].m_startIndex = startIndex;

    return mPrimGroups;
}

void Mesh::setPrimitiveGroups( size_t numPrimGroups, PrimitiveGroup *primGroups ) {
    mNumPrimGroups = numPrimGroups;
    mPrimGroups = primGroups;
}

} // Namespace RenderBackend
} // Namespace OSRE
