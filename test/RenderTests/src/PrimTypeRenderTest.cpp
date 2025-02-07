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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include "Common/Logger.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/MeshBuilder.h"

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  A primitive meshes - rendering test
//-------------------------------------------------------------------------------------------------
class PrimTypeRenderTest final : public AbstractRenderTest {
    static constexpr ui32 NumPoints = 3;
    static constexpr ui32 PtNumIndices = 3;

    TransformMatrixBlock mTransformMatrix;

public:
    PrimTypeRenderTest() : AbstractRenderTest("rendertest/PrimTypeRenderTest") {
        // empty
    }

    ~PrimTypeRenderTest() override = default;

    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        // colors
        glm::vec3 col[NumPoints] = {};
        col[0] = glm::vec3(1, 0, 0);
        col[1] = glm::vec3(0, 1, 0);
        col[2] = glm::vec3(0, 0, 1);

        // point coordinates
        glm::vec3 points[NumPoints] = {};
        points[0] = glm::vec3(-0.5, -0.5, 0);
        points[1] = glm::vec3(0, 0.5, 0);
        points[2] = glm::vec3(0.5, -0.5, 0);

        // line segment coordinates
        glm::vec3 pos[NumPoints] = {};
        pos[0] = glm::vec3(-1, -1, 0);
        pos[1] = glm::vec3(0, 1, 0);
        pos[2] = glm::vec3(1, -1, 0);

        static const ui32 NumIndices = 6;
        ui16 indices[NumIndices] = {};
        indices[0] = 0;
        indices[1] = 1;

        indices[2] = 1;
        indices[3] = 2;

        indices[4] = 2;
        indices[5] = 0;

        MeshBuilder meshBuilder;
        meshBuilder.allocPoints(VertexType::ColorVertex, BufferAccessType::ReadOnly, NumPoints, points, col);
        Mesh *ptMesh = meshBuilder.getMesh();

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("batch1");
            {
                rbSrv->addMesh(ptMesh, 0);
                meshBuilder.allocEmptyMesh("empty_mesh", VertexType::ColorVertex);
                Mesh *lineMesh = meshBuilder.getMesh();
                MeshBuilder::allocVertices(lineMesh, VertexType::ColorVertex, 3, pos, col, nullptr, BufferAccessType::ReadOnly);
                size_t size = sizeof(ui16) * static_cast<size_t>(NumIndices);
                lineMesh->createIndexBuffer(indices, size, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

                // setup primitives
                lineMesh->addPrimitiveGroup(6, PrimitiveType::LineList, 0);

                // setup material
                lineMesh->setMaterial(MaterialBuilder::createBuildinMaterial(VertexType::ColorVertex));
                rbSrv->addMesh(lineMesh, 0);

                mTransformMatrix.mModel = glm::rotate(mTransformMatrix.mModel, 0.0f, glm::vec3(1, 1, 0));
                mTransformMatrix.mModel = glm::scale(mTransformMatrix.mModel, glm::vec3(.5, .5, .5));
                rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST(PrimTypeRenderTest)

} // Namespace RenderTest
} // Namespace OSRE
