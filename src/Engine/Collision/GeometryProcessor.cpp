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
#include <osre/Collision/GeometryProcessor.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Geometry.h>

namespace OSRE {
namespace Collision {

using namespace ::OSRE::RenderBackend;

static const i32 NeedsUpdate = 1;

GeometryProcessor::GeometryProcessor()
: AbstractProcessor()
, m_geoArray()
, m_aabb()
, m_dirty( 0 ) {
    // empty
}
        
GeometryProcessor::~GeometryProcessor() {
    // empty
}
        
bool GeometryProcessor::execute() {
    if ( m_geoArray.isEmpty() ) {
        return true;
    }


    if ( m_dirty &= NeedsUpdate ) {
        for ( ui32 i = 0; i < m_geoArray.size(); i++ ) {
            handleGeometry( m_geoArray[ i ] );
        }
    }

    m_dirty = 0;

    return true;
}

void GeometryProcessor::addGeo( RenderBackend::Geometry *geo ) {
    if ( nullptr == geo ) {
        return;
    }
    
    m_geoArray.add( geo );
    
    m_dirty |= NeedsUpdate;
}
        
const Scene::Node::AABB &GeometryProcessor::getAABB() const {
    return m_aabb;
}

void GeometryProcessor::handleGeometry( RenderBackend::Geometry *geo ) {
    OSRE_ASSERT( nullptr != geo );

    if ( nullptr == geo ) {
        return;
    }

    ui32 offsetPos = 0, stride = 0;
    switch ( geo->m_vertextype ) {
        case VertexType::RenderVertex:
            offsetPos = 0;
            stride = sizeof( RenderVert );
            break;

        case VertexType::ColorVertex:
            offsetPos = 0;
            stride = sizeof( ColorVert );
            break;

        default:
            break;
    }

    BufferData *data = geo->m_vb;
    if ( nullptr == data || 0 == data->m_size ) {
        return;
    }

    ui32 offset( 0 );
    const ui32 numVertices = data->m_size / stride;
    for ( ui32 i = 0; i < numVertices; i++ ) {
        glm::vec3 pos;
        uc8 *ptr = (uc8*) data->m_data;
        ::memcpy( &pos.x, &ptr[offset], sizeof( glm::vec3 ) );
        offset += stride;
        m_aabb.merge( pos.x, pos.y, pos.z );
    }
}

} // Namespace Collision
} // Namespace OSRE
