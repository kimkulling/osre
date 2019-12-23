#pragma once

#include <osre/Common/Object.h>

namespace OSRE {

namespace Scene {
    class Node;
}

namespace App {

class AbstractBehaviour;

class GameObject : public Common::Object {
public:
    GameObject( const String &name );
    ~GameObject();
    void setBehaviourControl(AbstractBehaviour *behaviour );
    void setNode( Scene::Node *node );

private:
    AbstractBehaviour *m_behaviour;
    Scene::Node *m_node;
};

} // Namespace App
} // Namespace OSRE
