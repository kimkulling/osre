#include <osre/App/Entity.h>
#include <osre/App/AbstractBehaviour.h>
#include <osre/App/Component.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Scene;

Entity::Entity( const String &name, const Common::Ids &ids )
: Object( name )
, m_behaviour( nullptr ) 
, m_renderComponent( nullptr )
, m_node( nullptr )
, m_ids( ids )
, m_aabb() {
    m_renderComponent = new RenderComponent( this, 1 );
}

Entity::~Entity() {
    // empty
}
 
void Entity::setBehaviourControl( AbstractBehaviour *behaviour ) {
    m_behaviour = behaviour;
}

void Entity::setNode( Node *node ) {
    m_node = node;
}

Node *Entity::getNode() const {
    return m_node;
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
        case OSRE::App::ComponentType::RenderComponentType:
            return m_renderComponent;
        case OSRE::App::ComponentType::TransformComponentType:
            break;
        case OSRE::App::ComponentType::ScriptComponent:
            break;
        case OSRE::App::ComponentType::MaxNumComponents:
            break;
        case OSRE::App::ComponentType::InvalidComponent:
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
