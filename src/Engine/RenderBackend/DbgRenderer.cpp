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
#include <osre/Debugging/osre_debugging.h>
#include <osre/Common/BaseMath.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/DbgRenderer.h>
#include <osre/RenderBackend/MaterialBuilder.h>
#include <osre/RenderBackend/MeshBuilder.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

DbgRenderer *DbgRenderer::sInstance = nullptr;

DbgRenderer::DbgRenderer(RenderBackendService *rbSrv) :
        mRbSrv(rbSrv),
        mDebugMesh(nullptr),
        mDebugTextMeshes(),
        mLastIndex(0) {
    osre_assert(nullptr != mRbSrv);
}

DbgRenderer::~DbgRenderer() {
    clear();
}

void DbgRenderer::render() {

}

bool DbgRenderer::create(RenderBackendService *rbSrv) {
    if (nullptr != sInstance) {
        return false;
    }

    sInstance = new DbgRenderer(rbSrv);
    return true;
}

bool DbgRenderer::destroy() {
    if (nullptr == sInstance) {
        return false;
    }
    delete sInstance;
    sInstance = nullptr;
    return true;
}

DbgRenderer *DbgRenderer::getInstance() {
    return sInstance;
}

const c8 *DbgRenderer::getDebugRenderBatchName() {
    static constexpr c8 name[] = "dbgBatch";
    return name;
}

void DbgRenderer::renderDbgText(ui32 x, ui32 y, guid id, const String &text) {
    osre_assert(mRbSrv != nullptr);
    
    if (text.empty()) {
        return;
    }
    DebugText *foundDebugText = getInstance()->getDebugText(id);
    mRbSrv->beginPass(RenderPass::getPassNameById(DbgPassId));
    mRbSrv->beginRenderBatch(DbgRenderer::getDebugRenderBatchName());
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    mRbSrv->setMatrix(MatrixType::Projection, projection);
    if (foundDebugText == nullptr) {
        MeshBuilder meshBuilder;
        meshBuilder.allocTextBox(static_cast<f32>(x), static_cast<f32>(y), 20, text, BufferAccessType::ReadWrite);
        DebugText *entry = new DebugText;
        entry->mesh = meshBuilder.getMesh();
        entry->mesh->setId(id);
        getInstance()->mDebugTextMeshes.add(entry);
        mRbSrv->setMatrix(MatrixType::Projection, projection);
        mRbSrv->addMesh(entry->mesh, 0);
    } else if (foundDebugText->text != text) {
        MeshBuilder::updateTextBox(foundDebugText->mesh, 10, text);
        mRbSrv->updateMesh(foundDebugText->mesh);
    }
    mRbSrv->endRenderBatch();
    mRbSrv->endPass();
}

static constexpr size_t NumIndices = 24;

static ui16 indices[NumIndices] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 4,
    1, 5,
    2, 6,
    3, 7
};

void DbgRenderer::renderAABB(const glm::mat4 &transform, const AABB &aabb) {
    osre_assert(mRbSrv != nullptr);

    MeshBuilder meshBuilder;
    meshBuilder.allocEmptyMesh("aabb", VertexType::ColorVertex);
    Mesh *mesh = meshBuilder.getMesh();

    static const ui32 NumVertices = 8;
    ColorVert vertices[NumVertices];
    const glm::vec3 &min(aabb.getMin());
    const glm::vec3 &max(aabb.getMax());
    f32 x0(min.x), y0(min.y), z0(min.z), x1(max.x), y1(max.y), z1(max.z);
    vertices[0].position.x = x0;
    vertices[0].position.y = y0;
    vertices[0].position.z = z0;

    vertices[1].position.x = x1;
    vertices[1].position.y = y0;
    vertices[1].position.z = z0;

    vertices[2].position.x = x1;
    vertices[2].position.y = y1;
    vertices[2].position.z = z0;

    vertices[3].position.x = x0;
    vertices[3].position.y = y1;
    vertices[3].position.z = z0;

    vertices[4].position.x = x0;
    vertices[4].position.y = y0;
    vertices[4].position.z = z1;

    vertices[5].position.x = x1;
    vertices[5].position.y = y0;
    vertices[5].position.z = z1;

    vertices[6].position.x = x1;
    vertices[6].position.y = y1;
    vertices[6].position.z = z1;

    vertices[7].position.x = x0;
    vertices[7].position.y = y1;
    vertices[7].position.z = z1;

    const size_t vertexSize(sizeof(ColorVert) * NumVertices);
    mesh->createVertexBuffer(&vertices[0], vertexSize, BufferAccessType::ReadOnly);
    const size_t indexSize(sizeof(ui16) * NumIndices);
    mesh->createIndexBuffer(&indices[0], indexSize, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

    // setup primitives
    mesh->setModelMatrix(false, transform);

    mesh->addPrimitiveGroup(NumIndices, PrimitiveType::LineList, 0);

    // setup material
    mesh->setMaterial(MaterialBuilder::createBuildinMaterial(VertexType::ColorVertex));

    mRbSrv->beginPass(RenderPass::getPassNameById(DbgPassId));
    mRbSrv->beginRenderBatch(DbgRenderer::getDebugRenderBatchName());

    mRbSrv->setMatrix(MatrixType::Model, transform);
    mRbSrv->addMesh(mesh, 0);

    mRbSrv->endRenderBatch();
    mRbSrv->endPass();
}

void DbgRenderer::clear() {
    delete mDebugMesh;
    mDebugMesh = nullptr;
}

void DbgRenderer::addLine(const ColorVert &v0, const ColorVert &v1) {
    if (nullptr == mDebugMesh) {
        mDebugMesh = new Mesh("debugMesh", VertexType::ColorVertex, IndexType::UnsignedShort);
    }

    ColorVert vertices[2];
    vertices[0] = v0;
    vertices[1] = v1;
    BufferData *vb = mDebugMesh->getVertexBuffer();
    if (vb == nullptr) {
        mDebugMesh->createVertexBuffer(&vertices[0], sizeof(ColorVert) * 2, RenderBackend::BufferAccessType::ReadOnly);
    } else {
        vb->attach(&vertices[0], sizeof(ColorVert) * 2);
    }
    ui16 lineIndices[2] = {};
    lineIndices[0] = mLastIndex;
    mLastIndex++;
    lineIndices[1] = mLastIndex;
    mLastIndex++;
    BufferData *ib = mDebugMesh->getIndexBuffer();
    if (ib == nullptr) {
        mDebugMesh->createIndexBuffer(&lineIndices[0], sizeof(ui16) * 2, IndexType::UnsignedShort, BufferAccessType::ReadOnly);
    } else {
        ib->attach(&lineIndices[0], sizeof(ui16) * 2);
    }

    mDebugMesh->addPrimitiveGroup(NumIndices, PrimitiveType::LineList, 0);
}

DbgRenderer::DebugText *DbgRenderer::getDebugText(guid id) const {
    if (mDebugTextMeshes.isEmpty()) {
        return nullptr;
    }

    DebugText *found = nullptr;
    for (size_t i = 0; i < mDebugTextMeshes.size(); ++i) {
        if (mDebugTextMeshes[i]->mesh->getId() == id) {
            found = mDebugTextMeshes[i];
            break;
        }
    }

    return found;
}

} // Namespace RenderBackend
} // namespace OSRE
