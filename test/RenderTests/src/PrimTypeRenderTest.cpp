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

        // colors
        glm::vec3 col[ 3 ];
        col[ 0 ] = glm::vec3( 1, 0, 0 );
        col[ 1 ] = glm::vec3( 0, 1, 0 );
        col[ 2 ] = glm::vec3( 0, 0, 1 );

        // point coordinates
        glm::vec3 points[ 3 ];
        points[ 0 ] = glm::vec3( -0.5, -0.5, 0 );
        points[ 1 ] = glm::vec3(  0,    0.5, 0 );
        points[ 2 ] = glm::vec3(  0.5, -0.5, 0 );
        static const ui32 PtNumIndices = 3;
        GLushort pt_indices[ PtNumIndices ];
        pt_indices[ 0 ] = 0;
        pt_indices[ 1 ] = 1;
        pt_indices[ 2 ] = 2;

        // line segment coordinates
        glm::vec3 pos[ 3 ];
        pos[ 0 ] = glm::vec3( -1, -1, 0 );
        pos[ 1 ] = glm::vec3(  0,  1, 0 );
        pos[ 2 ] = glm::vec3(  1, -1, 0 );

        static const ui32 NumIndices = 6;
        GLushort indices[ NumIndices ];
        indices[ 0 ] = 0;
        indices[ 1 ] = 1;

        indices[ 2 ] = 1;
        indices[ 3 ] = 2;

        indices[ 4 ] = 2;
        indices[ 5 ] = 0;

        static ui32 NumGeo( 2 );
        attachGeoEvData->m_numGeo = NumGeo;
        attachGeoEvData->m_geo = Scene::GeometryBuilder::allocEmptyGeometry( VertexType::ColorVertex, NumGeo );
        
        Geometry *ptGeo = &attachGeoEvData->m_geo[ 0 ];
        ptGeo->m_vb = Scene::GeometryBuilder::allocVertices( VertexType::ColorVertex, 3, points, col, nullptr, BufferAccessType::ReadOnly );
        ptGeo->m_indextype = IndexType::UnsignedShort;
        ui32 pt_size = sizeof( GLushort ) * PtNumIndices;
        ptGeo->m_ib = BufferData::alloc( BufferType::IndexBuffer, pt_size, BufferAccessType::ReadOnly );
        ptGeo->m_ib->copyFrom( pt_indices, pt_size );
        
        // setup primitives
        ptGeo->m_numPrimGroups = 1;
        ptGeo->m_pPrimGroups = new PrimitiveGroup[ ptGeo->m_numPrimGroups ];
        ptGeo->m_pPrimGroups[ 0 ].init( IndexType::UnsignedShort, 3, PrimitiveType::PointList, 0 );

        Geometry *lineGeo = &attachGeoEvData->m_geo[ 1 ];
        lineGeo->m_vb = Scene::GeometryBuilder::allocVertices( VertexType::ColorVertex, 3, pos, col, nullptr, BufferAccessType::ReadOnly );
        lineGeo->m_indextype = IndexType::UnsignedShort;
        ui32 size = sizeof( GLushort ) * NumIndices;
        lineGeo->m_ib = BufferData::alloc( BufferType::IndexBuffer, size, BufferAccessType::ReadOnly );
        lineGeo->m_ib->copyFrom( indices, size );

        // setup primitives
        lineGeo->m_numPrimGroups = 1;
        lineGeo->m_pPrimGroups = new PrimitiveGroup[ lineGeo->m_numPrimGroups ];
        lineGeo->m_pPrimGroups[ 0 ].init( IndexType::UnsignedShort, 2*3, PrimitiveType::LineList, 0 );
        
        // setup material
        Material *mat = Scene::MaterialBuilder::createBuildinMaterial( VertexType::ColorVertex );
        ptGeo->m_material = mat;
        lineGeo->m_material = mat;
        
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.m_model = glm::scale( m_transformMatrix.m_model, glm::vec3( .5, .5, .5 ) );
        Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );

        mat->m_numParameters = 1;
        mat->m_parameters = parameter;

        rb->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

        return true;
    }
};

ATTACH_RENDERTEST( PrimTypeRenderTest )

} // Namespace RenderTest
} // Namespace OSRE