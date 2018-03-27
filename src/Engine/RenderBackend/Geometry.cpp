#include <osre/RenderBackend/Geometry.h>
#include <osre/Common/Logger.h>
#include <osre/Common/Ids.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

// Id container used for geometries
static Ids s_Ids;

// The log tag for messages
static const String Tag = "Geometry";

Geometry::Geometry()
: m_model( 1.0f )
, m_material( nullptr )
, m_vertextype( VertexType::RenderVertex )
, m_vb( nullptr )
, m_ib( nullptr )
, m_numPrimGroups( 0 )
, m_pPrimGroups( nullptr )
, m_id( 99999999 ) {
    // empty
}

Geometry::~Geometry() {
    delete m_material;
    m_material = nullptr;

    delete m_vb;
    m_vb = nullptr;

    delete m_ib;
    m_ib = nullptr;

    delete[] m_pPrimGroups;
    m_pPrimGroups = nullptr;

    s_Ids.releaseId( m_id );
}

Geometry *Geometry::create( ui32 numGeo ) {
    if ( 0 == numGeo ) {
        osre_debug( Tag, "Number of static geo to create is zero." );
        return nullptr;
    }
    Geometry *geoArray( new Geometry[ numGeo ] );
    for ( ui32 i = 0; i < numGeo; i++ ) {
        geoArray[ i ].m_id = s_Ids.getUniqueId();
    }
    return geoArray;
}

void Geometry::destroy( Geometry **geo ) {
    delete[] * geo;
    ( *geo ) = nullptr;
}

ui32 Geometry::getVertexSize( VertexType vertextype ) {
    ui32 vertexSize( 0 );
    switch ( vertextype ) {
    case VertexType::ColorVertex:
        vertexSize = sizeof( ColorVert );
        break;

    case VertexType::RenderVertex:
        vertexSize = sizeof( RenderVert );
        break;

    default:
        break;
    }

    return vertexSize;
}

} // Namespace RenderBackend
} // Namespace OSRE
