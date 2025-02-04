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

const String VsSrc =
        "#version 400 core\n"
        "\n"
        "layout(location = 0) in vec3 position;	            // object space vertex position\n"
        "layout(location = 1) in vec3 normal;	            // object space vertex normal\n"
        "layout(location = 2) in vec3 color0;	            // object space vertex normal\n"
        "layout(location = 3) in vec2 texcoord0;	        // texture coordinate\n"
        "\n"
        "//output from the vertex shader\n"
        "smooth out vec4 vSmoothColor\n;		            //smooth colour to fragment shader\n"
        "smooth out vec2 vUV;\n"
        "\n"
        "//uniform\n"
        "uniform mat4 MVP;	                                //combined modelview projection matrix\n"
        "\n"
        "void main() {\n"
        "    // assign the per-vertex color to vSmoothColor varying\n"
        "    vSmoothColor = vec4(color0,1);\n"
        "\n"
        "    // get the clip space position by multiplying the combined MVP matrix with the object space\n"
        "    // vertex position\n"
        "    gl_Position = MVP*vec4(position,1);\n"
        "    vUV = texcoord0;\n"
        "}\n";

const String FsSrc =
        "#version 400 core\n"
        "\n"
        "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
        "\n"
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		    //interpolated colour to fragment shader\n"
        "smooth in vec2 vUV;\n"
        "uniform sampler2D tex0;\n"
        "\n"
        "void main() {\n"
        "    //set the interpolated color as the shader output\n"
        "    vFragColor = texture( tex0, vUV );\n"
        "}\n";

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  This class implements a simple texture render test.
//-------------------------------------------------------------------------------------------------
class BaseTextureRenderTest : public AbstractRenderTest {
    f32 m_angle;
    TransformMatrixBlock m_transformMatrix;
    UniformVar *m_mvpParam;

public:
    BaseTextureRenderTest() :
            AbstractRenderTest("rendertest/basetexturerendertest"),
            m_angle(0.02f),
            m_transformMatrix(),
            m_mvpParam(nullptr) {
        // empty
    }

    ~BaseTextureRenderTest() override {
        UniformVar::destroy(m_mvpParam);
        m_mvpParam = nullptr;
    }

    bool onCreate(RenderBackendService *rbSrv) override {
        osre_debug(Tag, "BaseTextureRenderTest::onCreate");

        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        MeshBuilder geoBuilder;
        geoBuilder.allocQuads(VertexType::RenderVertex, BufferAccessType::ReadOnly);
        Mesh *mesh = geoBuilder.getMesh();

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {
                rbSrv->addMesh(mesh, 0);

                // use textured material
                TextureResourceArray texResArray;
                TextureResource *texRes = new TextureResource("SpiderTex", IO::Uri("file://assets/Models/Obj/SpiderTex.jpg"));
                texResArray.add(texRes);
                Material *material = MaterialBuilder::createTexturedMaterial("SpiderTex", texResArray,
                    VertexType::RenderVertex);
                mesh->setMaterial(material);
                Shader *shader = material->getShader();
                if (shader != nullptr) {
                    shader->addVertexAttribute("position");
                    shader->addVertexAttribute("normal");
                    shader->addVertexAttribute("color0");
                    shader->addVertexAttribute("texcoord0");

                    shader->addUniformBuffer("MVP");
                }

                m_transformMatrix.mModel = glm::rotate(m_transformMatrix.mModel, m_angle, glm::vec3(1, 1, 0));
                rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.mModel);
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
                m_transformMatrix.mModel = glm::rotate(m_transformMatrix.mModel, m_angle, glm::vec3(1, 1, 0));
                rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.mModel);
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
