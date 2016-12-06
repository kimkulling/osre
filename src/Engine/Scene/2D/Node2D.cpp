#include <osre/Scene/2D/Node2D.h>>
#include "Sprite.h"

namespace OSRE {
namespace Scene {

Node2D::Node2D( const String &name, Common::Ids &ids, bool transformEnabled, bool renderEnabled, Node *parent )
: Node( name, ids, transformEnabled, renderEnabled )
, m_sprite( nullptr ) {
    // empty
}

Node2D::~Node2D() {

}

void Node2D::setSprite( Sprite *sprite ) {
    m_sprite = sprite;
}

Sprite *Node2D::getSprite() const {
    return m_sprite;
}

} // Namespace Scene
} // Namespace OSRE
