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
#include <osre/UI/Panel.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/MaterialBuilder.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;

Panel::Panel( const String &name, Widget *parent )
: Widget( name, parent )
, m_angle( 0.02f )
,m_transformMatrix() {
}

Panel::~Panel() {

}

void Panel::onRender( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) {
    const RectUI &rect( getRect() );
    Geometry *geo = Geometry::create( 1 );
    AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;

    geo->m_vertextype = VertexType::RenderVertex;
    geo->m_indextype = IndexType::UnsignedShort;

    RenderVert vertices[ 4 ];
    ui16 indices[ 6 ];

    f32 x1, y1, x2, y2;
    WidgetCoordMapping::mapPosToWorld( rect.getX1(), rect.getY1(), x1, y1 );
    WidgetCoordMapping::mapPosToWorld( rect.getX2(), rect.getY2(), x2, y2 );

    // setup triangle vertices
/*    vertices[ 0 ].position = glm::vec3( -1, -1, 0 );
    vertices[ 1 ].position = glm::vec3( -1, 1, 0 );
    vertices[ 2 ].position = glm::vec3( 1, -1, 0 );
    vertices[ 3 ].position = glm::vec3( 1, 1, 0 );*/

    vertices[ 0 ].position = glm::vec3( x1, y1, 0 );
    vertices[ 1 ].position = glm::vec3( x1, y2, 0 );
    vertices[ 2 ].position = glm::vec3( x2, y1, 0 );
    vertices[ 3 ].position = glm::vec3( x2, y2, 0 );

    vertices[ 0 ].tex0 = glm::vec2( 0, 0 );
    vertices[ 1 ].tex0 = glm::vec2( 0, 1 );
    vertices[ 2 ].tex0 = glm::vec2( 1, 0 );
    vertices[ 3 ].tex0 = glm::vec2( 1, 1 );

    geo->m_vb = BufferData::alloc( BufferType::VertexBuffer, sizeof( RenderVert ) * 4, BufferAccessType::ReadOnly );
    geo->m_vb->copyFrom( vertices, geo->m_vb->m_size );

    // setup triangle indices
    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;

    indices[ 3 ] = 1;
    indices[ 4 ] = 2;
    indices[ 5 ] = 3;

    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, sizeof( ui16 ) * 6, BufferAccessType::ReadOnly );
    geo->m_ib->copyFrom( indices, geo->m_ib->m_size );

    attachGeoEvData->m_numGeo = 1;
    attachGeoEvData->m_geo = new Geometry*[ 1 ];
    attachGeoEvData->m_geo[ 0 ] = geo;

    // use default material
    geo->m_material = Scene::MaterialBuilder::createBuildinMaterial( VertexType::RenderVertex );

    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups = new PrimitiveGroup[ 1 ];
    geo->m_pPrimGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = 6;
    geo->m_pPrimGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    rbSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

    m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, m_angle, glm::vec3( 1, 1, 0 ) );

    Parameter *parameter = Parameter::create( "MVP", ParameterType::PT_Mat4 );
    m_transformMatrix.update();
    ::memcpy( parameter->m_data.m_data, m_transformMatrix.getMVP(), sizeof( glm::mat4 ) );

    geo->m_material->m_parameters = parameter;
    geo->m_material->m_numParameters++;

    targetGeoArray.add( geo );
}

} // Namespace UI
} // Namespace OSRE
