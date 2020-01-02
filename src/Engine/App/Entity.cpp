#include <osre/App/Entity.h>
#include <osre/App/AbstractBehaviour.h>
#include <osre/app/Component.h>

namespace OSRE {
namespace App {

Entity::Entity( const String &name )
: Object( name )
, m_behaviour( nullptr ) 
, m_renderComponent( nullptr )
, m_node( nullptr ) {
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

} // Namespace App
} // Namespace OSRE
