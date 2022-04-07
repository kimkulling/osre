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
#include <osre/Common/Logger.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Scene/LineBuilder.h>

//#include <GL/gl.h>
//#include <GL/glew.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

LineBuilder::LineBuilder() :
        m_posCache(),
        m_diffuseColCache(),
        m_normalCache(),
        m_tex0Cache(),
        m_activePrimGroup(nullptr),
        m_indexCache(),
        m_primGroupCache(),
        m_isDirty(),
        m_activeMesh(nullptr) {
    // empty
}

LineBuilder &LineBuilder::addLine(const glm::vec3 &pos0, const glm::vec3 &pos1) {
    m_posCache.add(pos0);
    m_posCache.add(pos1);

    preparePrimGroups();

    m_activePrimGroup->m_startIndex = m_indexCache.size();
    m_activePrimGroup->m_numIndices += 2;

    m_indexCache.add((ui32) m_activePrimGroup->m_startIndex);
    m_isDirty = true;

    return *this;
}

LineBuilder &LineBuilder::addLines(glm::vec3 *pos0, glm::vec3 *pos1, ui32 numLines) {
    for (ui32 i = 0; i < numLines; ++i) {
        glm::vec3 position0(pos0[i].x, pos0[i].y, pos0[i].z);
        m_posCache.add(position0);
        glm::vec3 position1(pos1[i].x, pos1[i].y, pos1[i].z);
        m_posCache.add(position1);
    }

    preparePrimGroups();

    m_activePrimGroup->m_startIndex = m_indexCache.size();
    m_activePrimGroup->m_numIndices += numLines;
    for (ui32 i = 0; i < numLines; ++i) {
        m_indexCache.add((ui32)m_activePrimGroup->m_startIndex + i);
    }
    m_isDirty = true;

    return *this;
}

Mesh *LineBuilder::getMesh() {
    if (!m_isDirty) {
        return m_activeMesh;
    }

    if (m_posCache.isEmpty()) {
        return m_activeMesh;
    }

    size_t size = 0;
    if (m_activeMesh->getVertexType() == VertexType::RenderVertex) {
        size = sizeof(RenderVert) * m_posCache.size();
    } else if (m_activeMesh->getVertexType() == VertexType::ColorVertex) {
        size = sizeof(ColorVert) * m_posCache.size();
    }

    c8 *ptr  = (c8*) m_activeMesh->mapVertexBuffer(size, BufferAccessType::ReadOnly);
    ui32 offset = 0;
    for (ui32 i = 0; i < m_posCache.size(); ++i) {
        RenderVert v;
        v.position = m_posCache[i];
        if (!m_diffuseColCache.isEmpty()) {
            v.color0 = m_normalCache[i];
        }
        if (!m_normalCache.isEmpty()) {
            v.normal = m_normalCache[i];
        }
        if (!m_tex0Cache.isEmpty()) {
            v.tex0 = m_tex0Cache[i];
        }
        ::memcpy(&ptr[offset], &v, sizeof(RenderVert));
        offset += sizeof(RenderVert);
    }
    m_activeMesh->unmapVertexBuffer();

    // setup indices
    size = sizeof(ui16) * m_indexCache.size();
    m_activeMesh->createIndexBuffer(&m_indexCache[0], size, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

    // setup primitives
    for (size_t i = 0; i < m_primGroupCache.size(); ++i) {
        m_activeMesh->addPrimitiveGroup(m_primGroupCache[i]);
    }
    m_isDirty = false;

    return m_activeMesh;
}

void LineBuilder::preparePrimGroups() {
    PrimitiveGroup *pg = new PrimitiveGroup;
    pg->m_primitive = PrimitiveType::LineList;
    if (nullptr != m_activePrimGroup) {
        if (PrimitiveType::LineList != m_activePrimGroup->m_primitive) {
            m_primGroupCache.add(pg);
            m_activePrimGroup = pg;
            m_activePrimGroup->m_indexType = m_activeMesh->getIndexType();
        }
    } else {
        m_primGroupCache.add(pg);
        m_activePrimGroup = pg;
        m_activePrimGroup->m_indexType = m_activeMesh->getIndexType();
        m_activePrimGroup->m_startIndex = m_indexCache.size();
    }
}

} // Namespace Scene
} // Namespace OSRE
