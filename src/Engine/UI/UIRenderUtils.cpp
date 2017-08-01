#include "UIRenderUtils.h"

#include <osre/UI/Widget.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

Geometry *UIRenderUtils::createRectFromStyle( WidgetType type, const RectUI &rect, const Style &style ) {
    Geometry *geo = Geometry::create( 1 );

    geo->m_vertextype = VertexType::RenderVertex;
    geo->m_indextype = IndexType::UnsignedShort;

    RenderVert vertices[ 4 ];
    ui16 indices[ 6 ];

    f32 x1, y1, x2, y2;
    WidgetCoordMapping::mapPosToWorld( rect.getX1(), rect.getY1(), x1, y1 );
    WidgetCoordMapping::mapPosToWorld( rect.getX2(), rect.getY2(), x2, y2 );

    // setup triangle vertices
    vertices[ 0 ].position = glm::vec3( x1, y1, 0 );
    vertices[ 1 ].position = glm::vec3( x1, y2, 0 );
    vertices[ 2 ].position = glm::vec3( x2, y1, 0 );
    vertices[ 3 ].position = glm::vec3( x2, y2, 0 );

    Color4 col;
    if ( WidgetType::Panel == type ) {
        col = style.m_properties[ ( ui32 ) Style::ColorTable::BGColorPanel ];
    } else if ( WidgetType::Button == type ) {
        col = style.m_properties[ ( ui32 ) Style::ColorTable::BGColorWidget ];
    }
    vertices[ 0 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );
    vertices[ 1 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );
    vertices[ 2 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );
    vertices[ 3 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );

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

    // use default material
    geo->m_material = Scene::MaterialBuilder::createBuildinUIMaterial();

    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups = new PrimitiveGroup[ 1 ];
    geo->m_pPrimGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numIndices = 6;
    geo->m_pPrimGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    return geo;
}

} // Namespace UI
} // Namespace OSRE
