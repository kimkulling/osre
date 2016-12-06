#pragma once

#include <osre/Scene/Node.h>

namespace OSRE {
namespace Scene {

class Sprite;

class Node2D : public Node {
public:
    Node2D( const String &name, Common::Ids &ids, bool transformEnabled, bool renderEnabled, Node *parent = nullptr );
    virtual ~Node2D();
    virtual void setSprite( Sprite *sprite );
    virtual Sprite *getSprite() const;

private:
    Sprite *m_sprite;
};

} // Namespace Scene
} // Namespace OSRE

