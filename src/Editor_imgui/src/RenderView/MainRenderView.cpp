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
#include "RenderView/MainRenderView.h"
#include "Animation/AnimatorBase.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/Mesh.h"

namespace OSRE {
namespace Editor {

namespace Colors {
    static const glm::vec3 Black(0, 0, 0);
    static const glm::vec3 White(1, 1, 1);
    static const glm::vec3 Grey(0.5, 0.5, 0.5);
    static const glm::vec3 Red(1, 0, 0);
    static const glm::vec3 Green(0, 1, 0);
    static const glm::vec3 Blue(0, 0, 1);
} // namespace Colors

using namespace OSRE::RenderBackend;
using namespace OSRE::App;
using namespace OSRE::Animation;

MainRenderView::MainRenderView() :
        mEditorElements() {
    // empty
}

MainRenderView::~MainRenderView() {
    mEditorElements.clear();
}

Mesh *MainRenderView::createCoordAxis(ui32 size) {
    Mesh *axis = new Mesh("axis", VertexType::ColorVertex, IndexType::UnsignedShort);
    ColorVert v[6];
    v[0].position.x = v[0].position.y = v[0].position.z = 0;
    v[0].color0 = Colors::Red;

    v[1].position.x = static_cast<f32>(size);
    v[1].position.y = v[1].position.z = 0;
    v[1].color0 = Colors::Red;

    v[2].position.x = v[2].position.y = v[2].position.z = 0;
    v[2].color0 = Colors::Green;

    v[3].position.y = static_cast<f32>(size);
    v[3].position.x = v[3].position.z = 0;
    v[3].color0 = Colors::Green;

    v[4].position.x = v[4].position.y = v[4].position.z = 0;
    v[4].color0 = Colors::Blue;

    v[5].position.z = static_cast<f32>(size);
    v[5].position.x = v[5].position.y = 0;
    v[5].color0 = Colors::Blue;

    cppcore::TArray<RenderBackend::ColorVert> axisData;
    axisData.add(v, 6);

    axis->attachVertices(&axisData[0], sizeof(ColorVert) * axisData.size());

    cppcore::TArray<ui16> axisIndices;
    axisIndices.add(0);
    axisIndices.add(1);

    axisIndices.add(2);
    axisIndices.add(3);

    axisIndices.add(4);
    axisIndices.add(5);

    axis->attachIndices(&axisIndices[0], sizeof(ui16) * axisIndices.size());
    axis->addPrimitiveGroup(axisData.size(), PrimitiveType::LineList, 0);
    axis->setMaterial(MaterialBuilder::createBuildinMaterial(VertexType::ColorVertex));

    return axis;
}

Mesh *MainRenderView::createGrid(ui32 numLines) {
    if (0 == numLines) {
        return nullptr;
    }

    Mesh *grid = new Mesh("grid", VertexType::ColorVertex, IndexType::UnsignedShort);
    f32 currentX = -300.0f, currentY = -300.0f;
    f32 diffX = 600.0f / numLines;
    f32 diffY = 600.0f / numLines;
    cppcore::TArray<RenderBackend::ColorVert> lineData;
    cppcore::TArray<ui16> lineIndices;
    ui16 currentIndex = 0;
    for (ui32 x = 0; x < numLines + 1; ++x) {
        ColorVert v1, v2;
        v1.position.x = v2.position.x = currentX;
        currentX += diffX;

        v1.position.y = -300;
        v2.position.y = 300;

        v1.position.z = v2.position.z = 0.0f;
        v1.color0 = v2.color0 = Colors::Grey;

        lineData.add(v1);
        lineData.add(v2);
        lineIndices.add(currentIndex);
        ++currentIndex;
        lineIndices.add(currentIndex);
        ++currentIndex;
    }
    for (ui32 y = 0; y < numLines + 1; ++y) {
        ColorVert v1, v2;
        v1.position.x = -300;
        v2.position.x = 300;
        v1.position.y = v2.position.y = currentY;
        currentY += diffY;
        v1.position.z = v2.position.z = 0.0f;
        v1.color0 = v2.color0 = Colors::Grey;
        lineData.add(v1);
        lineData.add(v2);
        lineIndices.add(currentIndex);
        ++currentIndex;
        lineIndices.add(currentIndex);
        ++currentIndex;
    }
    grid->attachVertices(&lineData[0], sizeof(ColorVert) * lineData.size());
    grid->attachIndices(&lineIndices[0], sizeof(ui16) * lineIndices.size());
    grid->addPrimitiveGroup(lineData.size(), PrimitiveType::LineList, 0);
    grid->setMaterial(MaterialBuilder::createBuildinMaterial(VertexType::ColorVertex));

    return grid;
}

void MainRenderView::createRect2D(const Rect2ui &r, Mesh *mesh2D, Style &style) {
    if (nullptr == mesh2D) {
        return;
    }

    glm::vec2 p0(r.x1, r.y1), p1(r.getX1(), r.getY2()), p2(r.getX2(), r.getY2()), p3(r.getX2(), r.getY2());
    UIVert edges[4] = {};
    edges[0].position = p0;
    edges[1].position = p1;
    edges[2].position = p2;
    edges[3].position = p3;
    edges[0].color0 = style.BG.toVec4();
    edges[1].color0 = style.BG.toVec4();
    edges[2].color0 = style.BG.toVec4();
    edges[3].color0 = style.BG.toVec4();

    constexpr size_t NumIndices = 6;
    cppcore::TArray<ui16> indices;
    indices.resize(NumIndices);
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;

    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;

    mesh2D->attachVertices(&edges[0], sizeof(glm::vec2) * 4);
    mesh2D->attachIndices(&indices[0], sizeof(ui16) * NumIndices);
    mesh2D->addPrimitiveGroup(6, PrimitiveType::TriangleList, NumIndices);
}

static void drawJoint(Bone *currentBone) {
    if (currentBone == nullptr) {
        return;
    }

}

static void drawBone(Bone *currentBone) {
    if (currentBone == nullptr) {
        return;
    }

    if (currentBone->mParent != -1) {
        drawJoint(currentBone);
    }
}

void MainRenderView::createBonesFromSkeleton(Skeleton *skeleton) {
    if (skeleton == nullptr) {
        return;
    }
    for (ui32 i = 0; i < skeleton->mBones.size(); ++i) {
        Bone *currentBone = skeleton->mBones[i];
        drawJoint(currentBone);
        drawBone(currentBone);
    }
}

void MainRenderView::createEditorElements(RenderComponent *rc) {
    if (rc == nullptr) {
        return;
    }

    Mesh *grid = MainRenderView::createGrid(60);
    rc->addStaticMesh(grid);
    mEditorElements.add(grid);
    Mesh *axis = MainRenderView::createCoordAxis(100);
    rc->addStaticMesh(axis);
    mEditorElements.add(axis);
}

void MainRenderView::render( RenderBackendService *rbSrv, glm::mat4 model ) {
    rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
    rbSrv->beginRenderBatch("b1");

    rbSrv->setMatrix(MatrixType::Model, model);

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}

} // namespace Editor
} // namespace OSRE
