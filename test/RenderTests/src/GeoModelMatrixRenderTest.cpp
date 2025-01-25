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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include "RenderBackend/Mesh.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/DbgRenderer.h"
#include "RenderBackend/MeshBuilder.h"

#include <iomanip>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  A geo-model - rendering test
//-------------------------------------------------------------------------------------------------
class GeoModelMatrixRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;
    f32 mAngle;

public:
    GeoModelMatrixRenderTest() :
            AbstractRenderTest("rendertest/GeoModelMatrixRenderTest"), mAngle(0.0f) {
        // empty
    }

    ~GeoModelMatrixRenderTest() override = default;

    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        MeshBuilder myBuilder;
        TransformState transform;
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("batch_1");
            {
                glm::mat4 model(1);
                myBuilder.createTriangle(VertexType::ColorVertex, BufferAccessType::ReadOnly);
                Mesh *mesh1 = myBuilder.getMesh();

                transform.setTranslation(0.25f, 0, 0);
                transform.setScale(0.2f, 0.2f, 0.2f);
                transform.toMatrix(model);

                rbSrv->setMatrix(MatrixType::Model, model);
                rbSrv->addMesh(mesh1, 0);
            }
            rbSrv->endRenderBatch();

            rbSrv->beginRenderBatch("batch_2");
            {
                glm::mat4 model(1);
                myBuilder.createTriangle(VertexType::ColorVertex, BufferAccessType::ReadOnly);
                Mesh *mesh2 = myBuilder.getMesh();

                transform.setTranslation(-0.25f, 0, 0);
                transform.setScale(0.2f, 0.2f, 0.2f);
                transform.toMatrix(model);

                rbSrv->setMatrix(MatrixType::Model, model);
                rbSrv->addMesh(mesh2, 0);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }

    bool onRender(RenderBackendService *rbSrv) override {
        TransformState transform;

        glm::mat4 rot(1.0);
        mAngle += 0.01f;
        rot = glm::rotate(rot, mAngle, glm::vec3(1, 1, 0));

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("batch_1");
            {
                glm::mat4 model(1);
                transform.setTranslation(0.25f, 0, 0);
                transform.setScale(0.2f, 0.2f, 0.2f);
                transform.rotation = rot;
                transform.toMatrix(model);
                rbSrv->setMatrix(MatrixType::Model, model);
            }
            rbSrv->endRenderBatch();
            
            rbSrv->beginRenderBatch("batch_2");
            {
                glm::mat4 model(1);
                transform.setTranslation(-0.25f, 0, 0);
                transform.setScale(0.2f, 0.2f, 0.2f);
                transform.rotation = rot;
                transform.toMatrix(model);
                rbSrv->setMatrix(MatrixType::Model, model);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        
        return true;
    }
};

ATTACH_RENDERTEST(GeoModelMatrixRenderTest)

} // namespace RenderTest
} // namespace OSRE
