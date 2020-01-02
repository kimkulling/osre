#pragma once

#include <osre/Common/Object.h>
#include <osre/App/AppCommon.h>
namespace OSRE {

namespace Scene {
    class Node;
}

namespace App {

class AbstractBehaviour;
class RenderComponent;

class Entity : public Common::Object {
public:
    /// The type of component
    enum class ComponentType {
        RenderComponentType,     ///< Renderable component
        TransformComponentType,  ///< Transformable component
        CollisionComponent,      ///< For collision
        ScriptComponent,         ///< For scripting events 
        MaxNumComponents,
        InvalidComponent
    };

    Entity( const String &name );
    virtual ~Entity();
    virtual void setBehaviourControl(AbstractBehaviour *behaviour );
    virtual void setNode( Scene::Node *node );
    virtual bool preprocess();
    virtual bool update( Time dt );
    virtual bool render( RenderBackend::RenderBackendService *rbSrv );
    virtual bool postprocess();

private:
    AbstractBehaviour *m_behaviour;
    RenderComponent *m_renderComponent;
    Scene::Node *m_node;
};

} // Namespace App
} // Namespace OSRE
