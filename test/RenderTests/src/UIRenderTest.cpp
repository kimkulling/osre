/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

static const String Tag = "UiRenderTest";

class UiRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;

public:
    UiRenderTest()
        : AbstractRenderTest( "rendertest/UiRenderTest" ) {
        // empty
    }

    virtual ~UiRenderTest() {
        // empty
    }

    bool onCreate( RenderBackendService *rbSrv ) override {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );
        Geometry *geo = Scene::GeometryBuilder::allocTriangles( VertexType::ColorVertex, BufferAccessType::ReadOnly );
        rbSrv->attachGeo( geo, 0 );
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.update();
        rbSrv->setMatrix( "MVP", m_transformMatrix.m_mvp );

        return true;
    }
};

ATTACH_RENDERTEST( UiRenderTest )

}
}
