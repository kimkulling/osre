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
#include "RenderTestSuite.h"
#include "RenderTestUtils.h"

#include "Common/Logger.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/Material.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/Shader.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/OGLRenderer/OGLShader.h"

#include <iostream>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

static const c8 *Tag = "BasetexturerenderTest";

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  This class implements a simple texture render test.
//-------------------------------------------------------------------------------------------------
class BaseTextureRenderTest : public AbstractRenderTest {
    f32 mAngle;
    TransformMatrixBlock mTransformMatrix;
    UniformVar *mMvpParam;

public:
    BaseTextureRenderTest() :
            AbstractRenderTest("rendertest/basetexturerendertest"),
            mAngle(0.02f),
            mTransformMatrix(),
            mMvpParam(nullptr) {
        // empty
    }

    ~BaseTextureRenderTest() override {
        UniformVar::destroy(mMvpParam);
    }

    bool onCreate(RenderBackendService *rbSrv) override {
        osre_debug(Tag, "BaseTextureRenderTest::onCreate");

        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        MeshBuilder meshBuilder;
        meshBuilder.allocQuads(VertexType::RenderVertex, BufferAccessType::ReadOnly);
        Mesh *mesh = meshBuilder.getMesh();

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {
                rbSrv->addMesh(mesh, 0);

                // use textured material
                TextureResourceArray texResArray;
                TextureResource *texRes = new TextureResource("SpiderTex", IO::Uri("file://assets/Models/Obj/SpiderTex.jpg"));
                texResArray.add(texRes);
                Material *material = 
                    MaterialBuilder::createBuildinMaterial(
                        "SpiderTex", 
                        texResArray, 
                        VertexType::RenderVertex
                    );
                mesh->setMaterial(material);
                Shader *shader = material->getShader();
                if (shader != nullptr) {
                    shader->addVertexAttributes(RenderVert::getAttributes(), RenderVert::getNumAttributes());
                    shader->addUniformBuffer("MVP");
                }

                mTransformMatrix.mModel = glm::rotate(mTransformMatrix.mModel, mAngle, glm::vec3(1, 1, 0));
                rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }

    bool onDestroy(RenderBackendService *) override {
        osre_debug(Tag, "BaseTextureRenderTest::onDestroy");

        return true;
    }

    bool onRender(RenderBackendService *rbSrv) override {
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {
                mTransformMatrix.mModel = glm::rotate(mTransformMatrix.mModel, mAngle, glm::vec3(1, 1, 0));
                rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST(BaseTextureRenderTest)

} // Namespace RenderTest
} // namespace OSRE
