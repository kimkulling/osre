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
#include "App/ParticleEmitter.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/MaterialBuilder.h"

#include <cppcore/Random/RandomGenerator.h>

namespace OSRE::App {

using namespace ::OSRE::RenderBackend;

ParticleEmitter::ParticleEmitter(RenderBackendService *rbSrv) :
        mRbSrv(rbSrv),
        mNumPoints(0),
        mCol(nullptr),
        mPos(nullptr),
        mPtIndices(nullptr),
        mPtGeo(nullptr),
        mUseBounds(false),
        mBounds() {
    // empty
}

ParticleEmitter::~ParticleEmitter() {
    delete[] mCol;
    delete[] mPos;
}

void ParticleEmitter::init(ui32 numPoints) {
    mNumPoints = numPoints;
    mCol = new glm::vec3[mNumPoints];
    mPos = new glm::vec3[mNumPoints];

    cppcore::RandomGenerator generator;
    for (ui32 i = 0; i < mNumPoints; i++) {
        const f32 r = static_cast<f32>(generator.get(1, 100)) / 100.0f;
        const f32 g = static_cast<f32>(generator.get(1, 100)) / 100.0f;
        const f32 b = static_cast<f32>(generator.get(1, 100)) / 100.0f;
        mCol[i] = glm::vec3(r, g, b);

        const f32 x = static_cast<f32>(generator.get(0, 400) - 200) / 100.0f;
        const f32 y = static_cast<f32>(generator.get(0, 400) - 200) / 100.0f;
        const f32 z = static_cast<f32>(generator.get(0, 400) - 200) / 100.0f;
        mPos[i] = glm::vec3(x, y, z);
    }

    mPtIndices = new GLushort[mNumPoints];
    for (ui32 i = 0; i < mNumPoints; i++) {
        mPtIndices[i] = static_cast<GLushort>(i);
    }

    MeshBuilder meshBuilder;
    meshBuilder.allocEmptyMesh("", VertexType::ColorVertex);
    mPtGeo = meshBuilder.getMesh();
    mRbSrv->addMesh(mPtGeo, 0);
    MeshBuilder::allocVertices(mPtGeo, VertexType::ColorVertex, mNumPoints, mPos, mCol, nullptr, BufferAccessType::ReadOnly);
    ui32 pt_size = sizeof(GLushort) * mNumPoints;
    mPtGeo->createIndexBuffer(mPtIndices, pt_size, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

    // setup primitives
    mPtGeo->addPrimitiveGroup(mNumPoints, PrimitiveType::PointList, 0);
    mPtGeo->setModelMatrix(true, glm::mat4(1.0f));

    // setup material
    TextureResourceArray texResArray;
    Material *mat = MaterialBuilder::createBuildinMaterial("default.mat", texResArray, VertexType::ColorVertex);
    mPtGeo->setMaterial(mat);
}

void ParticleEmitter::update(d32 /*tick*/) {
    cppcore::RandomGenerator generator;
    for (ui32 i = 0; i < mNumPoints; i++) {
        const f32 x = static_cast<f32>(generator.get(-10, 10)) / 100.0f;
        const f32 y = static_cast<f32>(generator.get(-10, 10)) / 100.0f;
        const f32 z = static_cast<f32>(generator.get(-10, 10)) / 100.0f;
        mPos[i] += glm::vec3(x, y, z);
        if (mUseBounds) {
            glm::vec3 pt(mPos[i].x, mPos[i].y, mPos[i].z);
            if (!mBounds.isIn(pt)) {
                mPos[i] -= mPos[i];
                mPos[i] -= mPos[i];
            }
        }
    }

    ui32 offset(0);
    for (ui32 i = 0; i < mNumPoints; i++) {
        uc8 *ptr = (uc8 *)mPtGeo->getVertexBuffer()->getData();
        ::memcpy(&ptr[offset], &mPos[i], sizeof(glm::vec3));
        offset += sizeof(ColorVert);
    }
}

void ParticleEmitter::setBounds(const Common::AABB &bounds) {
    mUseBounds = true;
    mBounds = bounds;
}

Mesh *ParticleEmitter::getMesh() const {
    return mPtGeo;
}

} // namespace OSRE::App