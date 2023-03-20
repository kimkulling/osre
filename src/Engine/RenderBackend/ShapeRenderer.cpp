/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/ShapeRenderer.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/Shader.h>

namespace OSRE {
namespace RenderBackend {

static Mesh *createMesh(const String &name) {
    Mesh *mesh = new Mesh(name, VertexType::ColorVertex, IndexType::UnsignedInt);

    return mesh;
}

static void setVertexColor(ColorVert *v, size_t numVertx, glm::vec4 col) {
    for (size_t i = 0; i < numVertx; ++i) {
        v[i].color0 = col;
    }
}

ShapeRenderer::ShapeRenderer(RenderBackendService *rbSrv) :
        mRbSrv(rbSrv), mColor(), mShader(nullptr), mDirtyMode(Inited), mMesh(nullptr) {
    // empty
}

ShapeRenderer &ShapeRenderer::setColor(glm::vec4 color) {
    mColor = color;

    return *this;
}

ShapeRenderer &ShapeRenderer::setShader(Shader *shader) {
    mShader = shader;

    return *this;
}

ShapeRenderer &ShapeRenderer::addRect(f32 x, f32 y, f32 w, f32 h, bool filled) {
    if (mMesh == nullptr) {
        mMesh = createMesh("shape");
        mDirtyMode = Created;
    } else {
        mDirtyMode = Updated;
    }

    ColorVert v[4];
    v[0].position.x = x;
    v[0].position.y = y;
    v[1].position.x = x + w;
    v[1].position.y = y;
    v[2].position.x = x + w;
    v[2].position.y = y + h;
    v[3].position.x = x;
    v[3].position.y = y + h;
    ui32 indices[6] = { 0, 1, 2, 1, 2, 3 };
    setVertexColor(v, 4, mColor);
    mMesh->attachVertices(v, 4);
    mMesh->attachIndices(indices, 6);

    return *this;
}

void ShapeRenderer::commit() {
    if (mMesh == nullptr || mDirtyMode == Inited) {
        return;
    }

    if (mDirtyMode == Created) {
        mRbSrv->addMesh(mMesh, 0);
    } else {
        mRbSrv->updateMesh(mMesh);
    }
}

} // namespace RenderBackend
} // namespace OSRE
