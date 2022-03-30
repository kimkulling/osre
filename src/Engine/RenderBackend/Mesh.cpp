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
        m_model(1.0f),
        m_material(nullptr),
        m_vertextype(vertexType),
        m_vb(nullptr),
        m_indextype(indextype),
        m_ib(nullptr),
        m_numPrimGroups(0),
        m_primGroups(nullptr),
        m_id(99999999),
        mVertexData(),
        mIndexData(),
        m_lastIndex(0) {
    m_id = s_Ids.getUniqueId();
}

Mesh::~Mesh() {
    m_material = nullptr;

    BufferData::free(m_vb);
    m_vb = nullptr;

    BufferData::free(m_ib);
    m_ib = nullptr;

    delete[] m_primGroups;
    m_primGroups = nullptr;

    s_Ids.releaseId(m_id);
}

void *Mesh::mapVertexBuffer( size_t vbSize, BufferAccessType accessType ) {
    m_vb = BufferData::alloc(BufferType::VertexBuffer, vbSize, accessType);
    return m_vb->getData();
}

void Mesh::unmapVertexBuffer() {
    // empty
}

void Mesh::createVertexBuffer(void *vertices, size_t vbSize, BufferAccessType accessType) {
    m_vb = BufferData::alloc(BufferType::VertexBuffer, vbSize, accessType);
    m_vb->copyFrom(vertices, vbSize);
}

BufferData *Mesh::getVertexBuffer() const {
    return m_vb;
}

void Mesh::createIndexBuffer(void *indices, size_t ibSize, IndexType indexType, BufferAccessType accessType) {
    m_ib = BufferData::alloc(BufferType::IndexBuffer, ibSize, accessType);
    m_indextype = indexType;
    m_ib->copyFrom(indices, ibSize);
}

BufferData *Mesh::getIndexBuffer() const {
    return m_ib;
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

PrimitiveGroup *Mesh::createPrimitiveGroups(size_t numPrimGroups, IndexType *types, size_t *numIndices,
        PrimitiveType *primTypes, ui32 *startIndices) {
    if (0 == numPrimGroups || nullptr == types || nullptr == numIndices || nullptr == primTypes || nullptr == startIndices) {
        return nullptr;
    }

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

PrimitiveGroup *Mesh::createPrimitiveGroup(size_t numIndices, PrimitiveType primType, ui32 startIndex) {
    m_numPrimGroups = 1;
    m_primGroups = new PrimitiveGroup[m_numPrimGroups];
    m_primGroups[0].m_numIndices = numIndices;
    m_primGroups[0].m_primitive = primType;
    m_primGroups[0].m_startIndex = startIndex;

    return m_primGroups;
}

void Mesh::setPrimitiveGroups( size_t numPrimGroups, PrimitiveGroup *primGroups ) {
    m_numPrimGroups = numPrimGroups;
    m_primGroups = primGroups;
}

} // Namespace RenderBackend
} // Namespace OSRE
