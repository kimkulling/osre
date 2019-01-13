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
#include <osre/Scene/Node.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace Scene {
    
using namespace ::OSRE::RenderBackend;
using namespace ::CPPCore;

static const glm::vec3 Dummy = glm::vec3( -1, -1, -1);

Component::Component(Node *node, ui32 id )
: m_owner( node )
, m_id( id ) {
	// empty
}

Component::~Component() {
	// empty
}

RenderComponent::RenderComponent(Node *node, ui32 id )
: Component(node, id )
, m_newGeo() {
    // empty
}

RenderComponent::~RenderComponent() {
    // empty
}

void RenderComponent::update( Time ) {
    // empty
}

void RenderComponent::draw( RenderBackendService *renderBackendSrv ) {
    if( !m_newGeo.isEmpty() ) {
        for ( ui32 i = 0; i < m_newGeo.size(); i++ ) {
            renderBackendSrv->addMesh( m_newGeo[ i ], 0 );
        }
        m_newGeo.resize( 0 );
    }
}

void RenderComponent::addStaticGeometry( Mesh *geo ) {
    if ( nullptr == geo ) {
        return;
    }

    m_newGeo.add( geo );
}

ui32 RenderComponent::getNumGeometry() const {
    return m_newGeo.size();
}

Mesh *RenderComponent::getGeoAt(ui32 idx) const {
    return m_newGeo[idx];
}

TransformComponent::TransformComponent(Node *node, ui32 id)
: Component(node, id)
, m_dirty(NotDirty)
, m_localTransformState()
, m_transform( 1.0f ) {
	// empty
}

TransformComponent::~TransformComponent() {
	// empty
}

void TransformComponent::update( Time ) {
    if (m_dirty == NeedsTransform) {
        m_localTransformState.toMatrix(m_transform);
        m_dirty = NotDirty;
    }
}

void TransformComponent::draw( RenderBackendService *rbSrv ) {
    //rbSrv->setMatrix(MatrixType::Model, m_transform);
}

void TransformComponent::setTranslation( const glm::vec3 &pos ) {
    m_localTransformState.m_translate = glm::vec3( pos );
    m_dirty = NeedsTransform;
}

const glm::vec3 &TransformComponent::getTranslation() const {
    return m_localTransformState.m_translate;
}

void TransformComponent::setScale( const glm::vec3 &scale ) {
    m_localTransformState.m_scale = glm::vec3( scale );;
    m_dirty = NeedsTransform;
}

const glm::vec3 &TransformComponent::getScale() const {
    return m_localTransformState.m_scale;
}

void TransformComponent::setTransformationMatrix(const glm::mat4 &m) {
    m_transform = m;
}

const glm::mat4 &TransformComponent::getTransformationMatrix() const {
    return m_transform;
}

glm::mat4 TransformComponent::getWorlTransformMatrix() {
    glm::mat4 wt(1.0);
    for (const Node *node = getOwnerNode(); node != nullptr; node = node->getParent() ) {
        TransformComponent *comp = (TransformComponent*) node->getComponent(Node::ComponentType::TransformComponentType);
        if (nullptr != comp) {
            wt = comp->getTransformationMatrix() * wt;
        }
    }

    return wt;
}

const RenderBackend::TransformState &TransformComponent::getTransformState() const {
    return m_localTransformState;
}

CollisionComponent::CollisionComponent(Node *node, ui32 id )
: Component( node, id ) {
    // empty
}

CollisionComponent::~CollisionComponent() {
    // empty
}

void CollisionComponent::update( Time ) {
    // empty
}

LightComponent::LightComponent(Node *node, ui32 id)
: Component(node, id)
, m_light()
, m_radius(10.f) {
    // empty
}

LightComponent::~LightComponent() {
    // empty
}

void LightComponent::update(Time dt) {

}

void LightComponent::setLight(RenderBackend::Light &light) {
    m_light = light;
}

const RenderBackend::Light &LightComponent::getLight() const {
    return m_light;
}

void LightComponent::setRadius(f32 r) {
    m_radius = r;
}

f32 LightComponent::getRadius() const {
    return m_radius;
}


} // Namespace Scene
} // Namespace OSRE
