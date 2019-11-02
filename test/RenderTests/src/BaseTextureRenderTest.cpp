/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/Shader.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/MaterialBuilder.h>
#include <src/Engine/RenderBackend/OGLRenderer/OGLShader.h>

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
    BaseTextureRenderTest()
    : AbstractRenderTest( "rendertest/basetexturerendertest" )
    , m_angle( 0.02f )
    , m_transformMatrix()
    , m_mvpParam( nullptr ){
        // empty
    }

    virtual ~BaseTextureRenderTest( ) {
        UniformVar::destroy( m_mvpParam );
        m_mvpParam = nullptr;
    }

    bool onCreate( RenderBackendService *rbSrv ) override {
        osre_debug( Tag, "BaseTextureRenderTest::onCreate" );

        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );

        Scene::MeshBuilder geoBuilder;
        geoBuilder.allocQuads( VertexType::RenderVertex, BufferAccessType::ReadOnly );
        Mesh *mesh = geoBuilder.getMesh();
        
        rbSrv->beginPass( PipelinePass::getPassNameById( RenderPassId ) );
        rbSrv->beginRenderBatch("b1");
        rbSrv->addMesh( mesh, 0 );

        // use textured material
        TextureResourceArray texResArray;;
        TextureResource* texRes = new TextureResource("SpiderTex", IO::Uri("file://assets/Models/Obj/SpiderTex.jpg") );
        texResArray.add(texRes);
        mesh->m_material = Scene::MaterialBuilder::createTexturedMaterial("SpiderTex", texResArray, VsSrc, FsSrc);
        if (nullptr != mesh->m_material->m_shader) {
            mesh->m_material->m_shader->m_attributes.add("position");
            mesh->m_material->m_shader->m_attributes.add("normal");
            mesh->m_material->m_shader->m_attributes.add("color0");
            mesh->m_material->m_shader->m_attributes.add("texcoord0");

            mesh->m_material->m_shader->m_parameters.add("MVP");
        }

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, m_angle, glm::vec3( 1, 1, 0 ) );
        rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.m_model);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        return true;
    }

    bool onDestroy( RenderBackendService* ) override {
        osre_debug( Tag, "BaseTextureRenderTest::onDestroy" );

        return true;
    }

    bool onRender( RenderBackendService *rbSrv ) override {
        rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, m_angle, glm::vec3( 1, 1, 0 ) );
        rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.m_model);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST( BaseTextureRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
