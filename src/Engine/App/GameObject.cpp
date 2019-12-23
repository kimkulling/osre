#include <osre/App/GameObject.h>
#include <osre/App/AbstractBehaviour.h>

namespace OSRE {
namespace App {


GameObject::GameObject( const String &name )
: Object( name )
, m_behaviour( nullptr ) 
, m_node( nullptr ) {
    // empty
}

GameObject::~GameObject() {
    // empty
}
 
void GameObject::setBehaviourControl( AbstractBehaviour *behaviour ) {
    m_behaviour = behaviour;
}

void GameObject::setNode( Scene::Node *node ) {
    m_node = node;
}

} // Namespace App
} // Namespace OSRE
