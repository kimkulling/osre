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
#include <osre/App/AbstractBehaviour.h>
#include <osre/App/Component.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/Scene/MeshProcessor.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Scene;
using namespace ::OSRE::RenderBackend;

Entity::Entity(const String &name, const Common::Ids &ids, World *world) :
        Object(name),
        m_behaviour(nullptr),
        m_renderComponent(nullptr),
        m_node(nullptr),
        m_ids(ids),
        m_aabb(),
        mOwner(world) {
    m_renderComponent = new RenderComponent(this, 1);
    if (nullptr != world) {
        mOwner->addEntity(this);
    }
}

Entity::~Entity() {
    delete m_renderComponent;
    if (nullptr != mOwner) {
        mOwner->removeEntity(this);
    }
}

void Entity::setBehaviourControl(AbstractBehaviour *behaviour) {
    m_behaviour = behaviour;
}

void Entity::setNode(Node *node) {
    m_node = node;
}

Node *Entity::getNode() const {
    return m_node;
}

bool Entity::preprocess() {
    return true;
}

bool Entity::update(Time dt) {
    if (nullptr != m_behaviour) {
        m_behaviour->update(dt);
    }

    return true;
}

bool Entity::render(RenderBackend::RenderBackendService *rbSrv) {
    m_renderComponent->render(rbSrv);

    return true;
}

bool Entity::postprocess() {
    return true;
}

Component *Entity::getComponent(ComponentType type) const {
    switch (type) {
        case OSRE::App::ComponentType::RenderComponentType:
            return m_renderComponent;
        case OSRE::App::ComponentType::ScriptComponent:
            break;
        case OSRE::App::ComponentType::MaxNumComponents:
        case OSRE::App::ComponentType::InvalidComponent:
            break;
        default:
            break;
    }

    return nullptr;
}

void Entity::setAABB(const AABB &aabb) {
    m_aabb = aabb;
}

const AABB &Entity::getAABB() const {
    return m_aabb;
}

} // Namespace App
} // Namespace OSRE
