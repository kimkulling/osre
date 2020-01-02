#include <osre/App/Entity.h>
#include <osre/App/AbstractBehaviour.h>
#include <osre/app/Component.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Scene;

Entity::Entity( const String &name )
: Object( name )
, m_behaviour( nullptr ) 
, m_renderComponent( nullptr )
, m_node( nullptr )
, m_aabb() {
    m_renderComponent = new RenderComponent( this, 1 );
}

Entity::~Entity() {
    // empty
}
 
void Entity::setBehaviourControl( AbstractBehaviour *behaviour ) {
    m_behaviour = behaviour;
}

void Entity::setNode( Scene::Node *node ) {
    m_node = node;
}

bool Entity::preprocess() {
    return true;
}

bool Entity::update( Time dt ) {
    return true;
}

bool Entity::render( RenderBackend::RenderBackendService *rbSrv ) {
    m_renderComponent->render( rbSrv );

    return true;
}

bool Entity::postprocess() {
    return true;
}

Component *Entity::getComponent( ComponentType type ) const {
    switch (type) {
        case OSRE::App::Entity::ComponentType::RenderComponentType:
            return m_renderComponent;
        case OSRE::App::Entity::ComponentType::TransformComponentType:
            break;
        case OSRE::App::Entity::ComponentType::CollisionComponent:
            break;
        case OSRE::App::Entity::ComponentType::ScriptComponent:
            break;
        case OSRE::App::Entity::ComponentType::MaxNumComponents:
            break;
        case OSRE::App::Entity::ComponentType::InvalidComponent:
            break;
        default:
            break;
    }

    return nullptr;
}

void Entity::setAABB( const Node::AABB &aabb ) {
    m_aabb = aabb;
}

const Node::AABB &Entity::getAABB() const {
    return m_aabb;
}

} // Namespace App
} // Namespace OSRE
