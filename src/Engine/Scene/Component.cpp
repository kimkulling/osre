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
#include <osre/Scene/Component.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace Scene {
    
using namespace ::OSRE::RenderBackend;
using namespace ::CPPCore;

static const glm::vec3 Dummy = glm::vec3( -1, -1, -1);

Component::Component( ui32 id )
: m_id( id ) {
	// empty
}

Component::~Component() {
	// empty
}

RenderComponent::RenderComponent( ui32 id )
: Component( id )
, m_newGeo() {
    // empty
}

RenderComponent::~RenderComponent() {
    // empty
}

void RenderComponent::update( RenderBackendService *renderBackendSrv ) {
    if( !m_newGeo.isEmpty() ) {
        for ( ui32 i = 0; i < m_newGeo.size(); i++ ) {
            renderBackendSrv->attachGeo( m_newGeo[ i ], 0 );
        }
        m_newGeo.resize( 0 );
    }
}

void RenderComponent::addStaticGeometry( Geometry *geo ) {
    if ( nullptr == geo ) {
        return;
    }

    m_newGeo.add( geo );
}

ui32 RenderComponent::getNumGeometry() const {
    return m_newGeo.size();
}

Geometry *RenderComponent::getGeoAt(ui32 idx) const {
    return m_newGeo[idx];
}

TransformComponent::TransformComponent( ui32 id )
: Component( id )
, m_localTransform() {
	// empty
}

TransformComponent::~TransformComponent() {
	// empty
}

void TransformComponent::update( RenderBackendService *renderBackendSrv ) {
    glm::mat4 world;
    m_localTransform.toMatrix( world );
}

void TransformComponent::setPosition( const glm::vec3 &pos ) {
    m_localTransform.m_translate = glm::vec3( pos );
}

const glm::vec3 &TransformComponent::getPosition() const {
    return m_localTransform.m_translate;
}

void TransformComponent::setScale( const glm::vec3 &scale ) {
    m_localTransform.m_scale = glm::vec3( scale );;
}

const glm::vec3 &TransformComponent::getScale() const {
    return m_localTransform.m_scale;
}

CollisionComponent::CollisionComponent( ui32 id )
: Component( id ) {
    // empty
}

CollisionComponent::~CollisionComponent() {
    // empty
}

void CollisionComponent::update( RenderBackendService *rbSrv ) {
    // empty
}

} // Namespace Scene
} // Namespace OSRE
