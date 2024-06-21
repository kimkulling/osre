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

#include "Common/Logger.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/Material.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/Shader.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/OGLRenderer/OGLShader.h"

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

static constexpr c8 VsSrc[] =
        "#version 400 core\n"
        "\n"
        "layout(location = 0) in vec3 position;	     // object space vertex position\n"
        "layout(location = 1) in vec3 normal;	     // object space vertex normal\n"
        "layout(location = 2) in vec3 color0;        // per-vertex colour\n"
        "\n"
        "// output from the vertex shader\n"
        "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
        "\n"
        "// uniform\n"
        "uniform mat4 M[25];	// model matrix per instance\n"
        "uniform mat4 VP;	    // combined modelview projection matrix\n"
        "\n"
        "void main() {\n"
        "    //assign the per-vertex color to vSmoothColor varying\n"
        "    vSmoothColor = vec4(color0,1);\n"
        "\n"
        "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
        "    //vertex position\n"
        "    gl_Position = VP * M[gl_InstanceID] * vec4(position,1);\n"
        "}\n";

static constexpr c8 FsSrc[] =
        "#version 400 core\n"
        "\n"
        "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
        "\n"
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
        "\n"
        "void main() {\n"
        "    //set the interpolated color as the shader output\n"
        "    vFragColor = vSmoothColor;\n"
        "}\n";

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  A instancing render-call - rendering test
//-------------------------------------------------------------------------------------------------
class GeoInstanceRenderTest : public AbstractRenderTest {
    static constexpr ui32 NumInstances = 25;
    f32 m_angle;
    glm::mat4 m_mat[NumInstances];
    TransformMatrixBlock m_transformMatrix;

public:
    GeoInstanceRenderTest() :
            AbstractRenderTest("rendertest/geoinstancerendertest"),
            m_angle(0.02f),
            m_transformMatrix() {
        for (ui32 i = 0; i < NumInstances; ++i) {
            m_mat[i] = glm::mat4(1.0f);
        }
    }

    ~GeoInstanceRenderTest() override = default;

    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {
                MeshBuilder myBuilder;
                myBuilder.createTriangle(VertexType::ColorVertex, BufferAccessType::ReadOnly);
                Mesh *mesh = myBuilder.getMesh();
                rbSrv->addMesh(mesh, NumInstances);

                // use a default material
                mesh->setMaterial(AbstractRenderTest::createMaterial("ColorVertexMat", VsSrc, FsSrc));
                Shader *shader = mesh->getMaterial()->getShader();
                if (shader != nullptr) {
                    shader->addVertexAttribute("position");
                    shader->addVertexAttribute("normal");
                    shader->addVertexAttribute("color0");

                    shader->addUniformBuffer("VP");
                    shader->addUniformBuffer("M");
                }

                m_transformMatrix.mModel = glm::rotate(m_transformMatrix.mModel, 0.0f, glm::vec3(1, 1, 0));
                m_transformMatrix.update();

                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
                i32 idx = 0;
                f32 x(-2.0f), y(-2.0f);
                for (i32 i = 0; i < 5; ++i) {
                    x = -2.0f;
                    for (auto j = 0; j < 5; ++j) {
                        m_mat[idx] = glm::translate(scale, glm::vec3(x, y, 0.f));
                        x += 2.0f;
                        ++idx;
                    }
                    y += 2.0f;
                }

                rbSrv->setMatrix("VP", m_transformMatrix.mMvp);
                rbSrv->setMatrixArray("M", NumInstances, m_mat);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }

    bool onRender(RenderBackendService *rbSrv) override {
        glm::mat4 rot(1.0);
        rot = glm::rotate(rot, m_angle, glm::vec3(1, 1, 0));
        for ( size_t i = 0; i < NumInstances; ++i ) {
            m_mat[i] = m_mat[i] * rot;
        }

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            {
                rbSrv->setMatrix("VP", m_transformMatrix.mMvp);
                rbSrv->setMatrixArray("M", NumInstances, m_mat);
            }
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST(GeoInstanceRenderTest)

} // Namespace RenderTest
} // Namespace OSRE
