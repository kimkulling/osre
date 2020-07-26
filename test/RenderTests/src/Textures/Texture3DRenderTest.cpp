/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/MeshBuilder.h>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

static const c8 *Tag = "Texture3DRenderTest";

class Texture3DRenderTest : public AbstractRenderTest {
    f32 m_angle;
    TransformMatrixBlock m_transformMatrix;

public:
    Texture3DRenderTest() :
            AbstractRenderTest("rendertest/texture3drendertest"),
            m_angle(0.02f),
            m_transformMatrix() {
        // empty
    }

    ~Texture3DRenderTest() override {
        // empty
    }

protected:
    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        Scene::MeshBuilder meshBuilder;
        meshBuilder.allocCube(VertexType::RenderVertex, 1, 1, 1, BufferAccessType::ReadOnly);
        Mesh *mesh = meshBuilder.getMesh();

        rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {
                rbSrv->addMesh(mesh, 0);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }

    bool onRender(RenderBackendService *rbSrv) override {
        rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {

                m_transformMatrix.m_model = glm::rotate(m_transformMatrix.m_model, m_angle, glm::vec3(1, 1, 0));
                rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.m_model);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST(Texture3DRenderTest)

} // namespace RenderTest
} // namespace OSRE
