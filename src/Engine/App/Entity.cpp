/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/CameraComponent.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/RenderBackend/MeshProcessor.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

Entity::Entity(const String &name, Common::Ids &ids, World *world) :
        Object(name),
        m_behaviour(nullptr),
        m_renderComponent(nullptr),
        mComponentArray(),
        m_node(nullptr),
        m_ids(ids),
        m_aabb(),
        mOwner(world) {
    mComponentArray.resize(Component::getIndex(ComponentType::MaxNumComponents));
    mComponentArray.set(nullptr);
    m_renderComponent = (RenderComponent*) createComponent(ComponentType::RenderComponentType);
    if (nullptr != mOwner) {
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

void Entity::setNode(TransformComponent *node) {
    m_node = node;
}

TransformComponent *Entity::getNode() const {
    return m_node;
}

bool Entity::update(Time dt) {
    if (nullptr != m_behaviour) {
        m_behaviour->update(dt);
    }
    for (auto &it : mComponentArray) {
        if (it != nullptr) {
            it->update(dt);
        }
    }

    return true;
}

bool Entity::render(RenderBackend::RenderBackendService *rbSrv) {
    for (auto &it : mComponentArray) {
        if (it != nullptr) {
            it->render(rbSrv);
        }
    }
    return true;
}

Component *Entity::createComponent(ComponentType type) {
    Component *component = getComponent(type);
    if (component != nullptr) {
        return component;
    }

    switch (type) {
        case OSRE::App::ComponentType::RenderComponentType:
            component = new RenderComponent(this);
            break;
        case OSRE::App::ComponentType::TransformComponentType: {
                const String name = getName() + "_transform";
                component = new TransformComponent(name, this, m_ids, nullptr); 
            }
            break;
        case OSRE::App::ComponentType::LightComponentType:
            component = new LightComponent(this);
            break;
        case OSRE::App::ComponentType::ScriptComponentType:
            component = new ScriptComponent(this);
            break;
        case OSRE::App::ComponentType::CameraComponentType:
            component = new Camera(this);
            break;
        case OSRE::App::ComponentType::InvalidComponent:
        case OSRE::App::ComponentType::MaxNumComponents:
            break;
        default:
            break;
    }
    mComponentArray[static_cast<size_t>(type)] = component;

    return component;
}

Component *Entity::getComponent(ComponentType type) const {
    if (type == ComponentType::MaxNumComponents || type == ComponentType::InvalidComponent) {
        return nullptr;
    }

    return mComponentArray[Component::getIndex(type)];
}

void Entity::setAABB(const AABB &aabb) {
    m_aabb = aabb;
}

const AABB &Entity::getAABB() const {
    return m_aabb;
}

void Entity::serialize( IO::Stream *stream ) {
    osre_assert(stream != nullptr);
}

void Entity::deserialize( IO::Stream *stream ) {
    osre_assert(stream != nullptr);
}


} // Namespace App
} // Namespace OSRE
