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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>
#include <osre/RenderBackend/MeshBuilder.h>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

// vertex array and vertex buffer object IDs
float angle = 0.0f;

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  This class implements a base triangle render test.
//-------------------------------------------------------------------------------------------------
class BaseTriangleRenderTest : public AbstractRenderTest {
    TransformMatrixBlock mTransformMatrix;

public:
    BaseTriangleRenderTest() :
            AbstractRenderTest("rendertest/basetrianglerendertest"), mTransformMatrix() {
        // empty
    }

    ~BaseTriangleRenderTest() override = default;

    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        CPPCore::TArray<Mesh *> meshArray;
        MeshBuilder meshBuilder;
        meshBuilder.createTriangle(VertexType::ColorVertex, BufferAccessType::ReadOnly);
        meshArray.add(meshBuilder.getMesh());

        mTransformMatrix.m_model = glm::rotate(mTransformMatrix.m_model, 0.0f, glm::vec3(1, 1, 0));

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {
                rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.m_model);
                rbSrv->addMesh(meshArray, 0);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST(BaseTriangleRenderTest)

} // Namespace RenderTest
} // Namespace OSRE
