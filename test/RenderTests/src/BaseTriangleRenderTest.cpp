/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <src/Engine/RenderBackend/OGLRenderer/OGLShader.h>
#include <osre/Scene/GeometryBuilder.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef WIN32
#   include "GL/wglew.h"
#endif

#include <iostream>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

// vertex array and vertex buffer object IDs
float angle = 0.0f;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Test
///
///	@brief  This class implements a base triangle render test.
//-------------------------------------------------------------------------------------------------
class BaseTriangleRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;

public:
    BaseTriangleRenderTest()
    : AbstractRenderTest( "rendertest/basetrianglerendertest" ) {
        // empty
    }

    virtual ~BaseTriangleRenderTest() {
        // empty
    }

    virtual bool onCreate( RenderBackendService *rbSrv ) override {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );
        
        CPPCore::TArray<Geometry*> geoArray;
        Geometry *geo = Scene::GeometryBuilder::allocTriangles( VertexType::ColorVertex, BufferAccessType::ReadOnly );
        geoArray.add( geo );
        
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.update();

        UniformVar *parameter = UniformVar::create( "MVP", ParameterType::PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, m_transformMatrix.getMVP(), sizeof( glm::mat4 ) );

        geo->m_material->m_parameters = parameter;
        geo->m_material->m_numParameters++;
        
        rbSrv->attachGeo( geoArray );

        return true;
    }
};

ATTACH_RENDERTEST( BaseTriangleRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
