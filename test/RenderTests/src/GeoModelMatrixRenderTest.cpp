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

#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/RenderBackend/Geometry.h>
#include <osre/Scene/DbgRenderer.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

class GeoModelMatrixRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;

public:
    GeoModelMatrixRenderTest()
    : AbstractRenderTest( "rendertest/GeoModelMatrixRenderTest" ) {
        // empty
    }

    virtual ~GeoModelMatrixRenderTest() {
        // empty
    }

    bool onCreate( RenderBackendService *rbSrv ) override {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );

        Scene::GeometryBuilder myBuilder;
        myBuilder.allocTriangles(VertexType::ColorVertex, BufferAccessType::ReadOnly);
        Geometry *geo1 = myBuilder.getGeometry();
        geo1->m_localMatrix = true; 
        TransformState transform;
        transform.setTranslation( 0.5f, 0, 0 );
        transform.setScale(0.2f, 0.2f, 0.2f);
        transform.toMatrix( geo1->m_model );
         
        rbSrv->beginPass();

        rbSrv->attachGeo( geo1, 0 );

        myBuilder.allocTriangles(VertexType::ColorVertex, BufferAccessType::ReadOnly);
        Geometry *geo2 = myBuilder.getGeometry();
        geo2->m_localMatrix = true;
        transform.setTranslation( -0.5f, 0, 0 );
        transform.setScale(0.2f, 0.2f, 0.2f);
        transform.toMatrix( geo2->m_model );
        rbSrv->attachGeo( geo2, 0 );

        rbSrv->endPass();

        return true;
    }
};

ATTACH_RENDERTEST( GeoModelMatrixRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
