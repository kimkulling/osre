/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include <osre/Common/Object.h>
#include <osre/App/AppCommon.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/AABB.h>
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
class AppBase;
class World;

class OSRE_EXPORT Entity : public Common::Object {
public:
    Entity( const String &name, const Common::Ids &ids, World *world );
    virtual ~Entity();
    virtual void setBehaviourControl(AbstractBehaviour *behaviour );
    virtual void addStaticMesh(RenderBackend::Mesh *mesh);
    virtual void addStaticMeshes(const RenderBackend::MeshArray &meshArray);
    virtual void setNode( Scene::Node *node );
    virtual Scene::Node *getNode() const;
    virtual bool preprocess();
    virtual bool update( Time dt );
    virtual bool render( RenderBackend::RenderBackendService *rbSrv );
    virtual bool postprocess();
    virtual Component *getComponent(ComponentType type) const;
    virtual void setAABB( const Scene::Node::AABB &aabb );
    virtual const Scene::Node::AABB &getAABB() const;

private:
    AbstractBehaviour *m_behaviour;
    RenderComponent *m_renderComponent;
    Scene::Node *m_node;
    const Common::Ids &m_ids;
    Scene::Node::AABB m_aabb;
    World *mOwner;
};

} // Namespace App
} // Namespace OSRE
