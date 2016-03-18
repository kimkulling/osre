/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <src/RenderBackend/OGLRenderer/OGLShader.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/MaterialBuilder.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

class PrimTypeRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;

public:
    PrimTypeRenderTest()
        : AbstractRenderTest( "rendertest/PrimTypeRenderTest" ) {
        // empty
    }

    virtual ~PrimTypeRenderTest() {
        // empty
    }

    virtual bool onCreate( RenderBackendService *rb ) {
        rb->sendEvent( &OnAttachViewEvent, nullptr );
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;

        glm::vec3 col[ 3 ];
        col[ 0 ] = glm::vec3( 1, 0, 0 );
        col[ 1 ] = glm::vec3( 0, 1, 0 );
        col[ 2 ] = glm::vec3( 0, 0, 1 );

        glm::vec3 pos[ 3 ];
        pos[ 0 ] = glm::vec3( -1, -1, 0 );
        pos[ 1 ] = glm::vec3( 0, 1, 0 );
        pos[ 2 ] = glm::vec3( 1, -1, 0 );

        static const ui32 NumIndices = 6;
        GLushort indices[ NumIndices ];
        indices[ 0 ] = 0;
        indices[ 1 ] = 1;

        indices[ 2 ] = 1;
        indices[ 3 ] = 2;

        indices[ 4 ] = 0;
        indices[ 5 ] = 2;

        Scene::GeometryBuilder myBuilder;
        StaticGeometry *geo = myBuilder.allocEmptyGeometry( ColorVertex );
        geo->m_vb = Scene::GeometryBuilder::allocVertices( ColorVertex, 3, pos, col, nullptr );
        geo->m_indextype = UnsignedShort;

        attachGeoEvData->m_numGeo = 1;
        attachGeoEvData->m_geo = geo;

        ui32 size = sizeof( GLushort ) * NumIndices;
        geo->m_ib = BufferData::alloc( IndexBuffer, size, ReadOnly );
        ::memcpy( geo->m_ib->m_pData, indices, size );

        // setup primitives
        geo->m_numPrimGroups = 1;
        geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
        geo->m_pPrimGroups[ 0 ].m_indexType = UnsignedShort;
        geo->m_pPrimGroups[ 0 ].m_numPrimitives = 3 * geo->m_numPrimGroups;
        geo->m_pPrimGroups[ 0 ].m_primitive = LineList;
        geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

        // setup material
        geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( ColorVertex );

        // setup shader attributes and variables
        if ( nullptr != geo->m_material->m_pShader ) {
            ui32 numAttribs( ColorVert::getNumAttributes() );
            const String *attribs( ColorVert::getAttributes() );
            geo->m_material->m_pShader->m_attributes.add( attribs, numAttribs );
            geo->m_material->m_pShader->m_parameters.add( "MVP" );
        }

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.m_model = glm::scale( m_transformMatrix.m_model, glm::vec3( .5, .5, .5 ) );
        Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );

        geo->m_parameter = parameter;
        geo->m_numParameter++;

        rb->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

        return true;
    }

    virtual bool onRender( d32 timediff, RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        return true;
    }
};

ATTACH_RENDERTEST( PrimTypeRenderTest )

}
}