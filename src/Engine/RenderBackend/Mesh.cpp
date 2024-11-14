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
#include "RenderBackend/Mesh.h"
#include "Common/Ids.h"
#include "Common/Logger.h"
#include "Debugging/osre_debugging.h"
#include "RenderBackend/Material.h"

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

// Id container used for geometries
static Ids s_Ids;

// The log tag for messages
static constexpr c8 Tag[] = "Mesh";

Mesh::Mesh(const String &name, VertexType vertexType, IndexType indextype) :
        mName(name),
        mLocalModelMatrix(false),
        mModel(1.0f),
        mMaterial(nullptr),
        mVertexType(vertexType),
        mVertexBuffer(nullptr),
        mIndexType(indextype),
        mIndexBuffer(nullptr),
        mPrimGroups(),
        mId(99999999),
        mVertexData(),
        mIndexData(),
        mLastIndex(0) {
    mId = s_Ids.getUniqueId();
}

Mesh::~Mesh() {
    s_Ids.releaseId(mId);
}

void *Mesh::mapVertexBuffer(size_t vbSize, BufferAccessType accessType) {
    mVertexBuffer = BufferData::alloc(BufferType::VertexBuffer, vbSize, accessType);
    return mVertexBuffer->getData();
}

void Mesh::unmapVertexBuffer() {
    // empty
}

void Mesh::createVertexBuffer(void *vertices, size_t vbSize, BufferAccessType accessType) {
    if (vertices == nullptr) {
        return;
    }

    mVertexBuffer = BufferData::alloc(BufferType::VertexBuffer, vbSize, accessType);
    mVertexBuffer->copyFrom(vertices, vbSize);
}

void Mesh::resizeVertexBuffer(size_t vbSize) {
    if (mVertexBuffer == nullptr) {
        return;
    }

    mVertexBuffer->m_buffer.resize(vbSize);
}


BufferData *Mesh::getVertexBuffer() const {
    return mVertexBuffer;
}

void Mesh::createIndexBuffer(void *indices, size_t ibSize, IndexType indexType, BufferAccessType accessType) {
    if (indices == nullptr) {
        return;
    }

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

void Mesh::addPrimitiveGroups(size_t numPrimGroups, size_t *numIndices, PrimitiveType *primTypes, ui32 *startIndices) {
    if (0 == numPrimGroups || nullptr == numIndices || nullptr == primTypes || nullptr == startIndices) {
        return;
    }

    const size_t index = mPrimGroups.size();
    for (size_t i = 0; i < numPrimGroups; ++i) {
        mPrimGroups.add(new PrimitiveGroup);
        mPrimGroups[index + i]->m_indexType = mIndexType;
        mPrimGroups[index + i]->m_numIndices = numIndices[i];
        mPrimGroups[index + i]->m_primitive = primTypes[i];
        mPrimGroups[index + i]->m_startIndex = startIndices[i];
    }

}

void Mesh::addPrimitiveGroup(size_t numIndices, PrimitiveType primType, ui32 startIndex) {
    if (numIndices == 0) {
        mPrimGroups.clear();
        return;
    }

    const size_t index = mPrimGroups.size();
    mPrimGroups.add(new PrimitiveGroup);
    mPrimGroups[index]->m_numIndices = numIndices;
    mPrimGroups[index]->m_indexType = mIndexType;
    mPrimGroups[index]->m_primitive = primType;
    mPrimGroups[index]->m_startIndex = startIndex;
}

void Mesh::addPrimitiveGroup( PrimitiveGroup *group ) {
    if (group == nullptr) {
        return;
    }

    mPrimGroups.add(group);
}

} // namespace RenderBackend
} // namespace OSRE
