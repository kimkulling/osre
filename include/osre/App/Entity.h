#pragma once

#include <osre/Common/Object.h>
#include <osre/App/AppCommon.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/TAABB.h>
#include <osre/App/Component.h>

namespace OSRE {

namespace Common {
    class Ids;
}

namespace Scene {
    class Node;
}

namespace App {

class AbstractBehaviour;
class Component;
class RenderComponent;
class TransformComponent;

class OSRE_EXPORT Entity : public Common::Object {
public:
    Entity( const String &name, const Common::Ids &ids );
    virtual ~Entity();
    virtual void setBehaviourControl(AbstractBehaviour *behaviour );
    virtual void addStaticMeshes( const RenderBackend::MeshArray &meshArray );
    virtual void setNode( Scene::Node *node );
    virtual Scene::Node *getNode() const;
    virtual bool preprocess();
    virtual bool update( Time dt );
    virtual bool render( RenderBackend::RenderBackendService *rbSrv );
    virtual bool postprocess();
    Component *getComponent( ComponentType type ) const;
    virtual void setAABB( const Scene::Node::AABB &aabb );
    virtual const Scene::Node::AABB &getAABB() const;

private:
    AbstractBehaviour *m_behaviour;
    RenderComponent *m_renderComponent;
    TransformComponent *m_transformComponent;
    Scene::Node *m_node;
    const Common::Ids &m_ids;
    Scene::Node::AABB m_aabb;
};

} // Namespace App
} // Namespace OSRE
