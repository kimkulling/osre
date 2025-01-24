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
#include "Common/Logger.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/LineBuilder.h"

namespace OSRE {
namespace Scene {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

LineBuilder::LineBuilder() :
        mPosCache(),
        mDiffuseColCache(),
        mNormalCache(),
        mTex0Cache(),
        mActivePrimGroup(nullptr),
        mIndexCache(),
        mPrimGroupCache(),
        mIsDirty(),
        mActiveMesh(nullptr) {
    // empty
}

LineBuilder &LineBuilder::addLine(const glm::vec3 &pos0, const glm::vec3 &pos1) {
    mPosCache.add(pos0);
    mPosCache.add(pos1);

    preparePrimGroups();

    mActivePrimGroup->startIndex = mIndexCache.size();
    mActivePrimGroup->numIndices += 2;

    mIndexCache.add((ui32) mActivePrimGroup->startIndex);
    mIsDirty = true;

    return *this;
}

LineBuilder &LineBuilder::addLines(glm::vec3 *pos0, glm::vec3 *pos1, ui32 numLines) {
    for (ui32 i = 0; i < numLines; ++i) {
        glm::vec3 position0(pos0[i].x, pos0[i].y, pos0[i].z);
        mPosCache.add(position0);
        glm::vec3 position1(pos1[i].x, pos1[i].y, pos1[i].z);
        mPosCache.add(position1);
    }

    preparePrimGroups();

    mActivePrimGroup->startIndex = mIndexCache.size();
    mActivePrimGroup->numIndices += numLines;
    for (ui32 i = 0; i < numLines; ++i) {
        mIndexCache.add((ui32)mActivePrimGroup->startIndex + i);
    }
    mIsDirty = true;

    return *this;
}

Mesh *LineBuilder::getMesh() {
    if (!mIsDirty) {
        return mActiveMesh;
    }

    if (mPosCache.isEmpty()) {
        return mActiveMesh;
    }

    size_t size = 0u;
    if (mActiveMesh->getVertexType() == VertexType::RenderVertex) {
        size = sizeof(RenderVert) * mPosCache.size();
    } else if (mActiveMesh->getVertexType() == VertexType::ColorVertex) {
        size = sizeof(ColorVert) * mPosCache.size();
    }

    c8 *ptr  = (c8*) mActiveMesh->mapVertexBuffer(size, BufferAccessType::ReadOnly);
    ui32 offset = 0u;
    for (ui32 i = 0; i < mPosCache.size(); ++i) {
        RenderVert v;
        v.position = mPosCache[i];
        if (!mDiffuseColCache.isEmpty()) {
            v.color0 = mNormalCache[i];
        }
        if (!mNormalCache.isEmpty()) {
            v.normal = mNormalCache[i];
        }
        if (!mTex0Cache.isEmpty()) {
            v.tex0 = mTex0Cache[i];
        }
        ::memcpy(&ptr[offset], &v, sizeof(RenderVert));
        offset += sizeof(RenderVert);
    }
    mActiveMesh->unmapVertexBuffer();

    // setup indices
    size = sizeof(ui16) * mIndexCache.size();
    mActiveMesh->createIndexBuffer(&mIndexCache[0], size, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

    // setup primitives
    for (size_t i = 0; i < mPrimGroupCache.size(); ++i) {
        mActiveMesh->addPrimitiveGroup(mPrimGroupCache[i]);
    }
    mIsDirty = false;

    return mActiveMesh;
}

void LineBuilder::preparePrimGroups() {
    PrimitiveGroup *pg = new PrimitiveGroup;
    pg->primitive = PrimitiveType::LineList;
    if (mActivePrimGroup != nullptr) {
        if (PrimitiveType::LineList != mActivePrimGroup->primitive) {
            mPrimGroupCache.add(pg);
            mActivePrimGroup = pg;
            mActivePrimGroup->indexType = mActiveMesh->getIndexType();
        }
    } else {
        mPrimGroupCache.add(pg);
        mActivePrimGroup = pg;
        mActivePrimGroup->indexType = mActiveMesh->getIndexType();
        mActivePrimGroup->startIndex = mIndexCache.size();
    }
}

} // Namespace Scene
} // Namespace OSRE
