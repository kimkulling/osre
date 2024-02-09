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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/DbgRenderer.h>
#include <osre/RenderBackend/MeshBuilder.h>

#include <iomanip>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Common;
using namespace ::OSRE::App;

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  A debug AABB - rendering test
//-------------------------------------------------------------------------------------------------
class AABBDbgRenderTest : public AbstractRenderTest {
    TransformMatrixBlock mTransformMatrix;

public:
    AABBDbgRenderTest() :
            AbstractRenderTest("rendertest/AABBDbgRenderTest") {
        // empty
    }

    ~AABBDbgRenderTest() override = default;

    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        glm::vec3 min(-1, -1, -1), max(1, 1, 1);
        AABB aabb(min, max);
        mTransformMatrix.mModel = glm::rotate(mTransformMatrix.mModel, 0.0f, glm::vec3(1, 1, 0));
        mTransformMatrix.mModel = glm::scale(mTransformMatrix.mModel, glm::vec3(.5, .5, .5));
        DbgRenderer::getInstance()->renderAABB(mTransformMatrix.mModel, aabb);

        return true;
    }

    bool onRender(RenderBackendService *rbSrv) override {
        rbSrv->beginPass(RenderPass::getPassNameById(DbgPassId));
        {
            rbSrv->beginRenderBatch(DbgRenderer::getDebugRenderBatchName());
            {
                mTransformMatrix.mModel = glm::rotate(mTransformMatrix.mModel, 0.01f, glm::vec3(1, 1, 0));
                rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST(AABBDbgRenderTest)

} // namespace RenderTest
} // namespace OSRE
