#include <osre/App/Entity.h>
#include <osre/App/AbstractBehaviour.h>

namespace OSRE {
namespace App {

Entity::Entity( const String &name )
: Object( name )
, m_behaviour( nullptr ) 
, m_node( nullptr ) {
    // empty
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
}

bool Entity::render() {
}

bool Entity::postprocess() {
}

} // Namespace App
} // Namespace OSRE
