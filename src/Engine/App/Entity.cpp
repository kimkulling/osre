/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/Entity.h"
#include "App/Component.h"
#include "App/CameraComponent.h"
#include "App/Scene.h"
#include "Animation/AnimatorComponent.h"
#include "RenderBackend/MeshProcessor.h"
#include "Common/Logger.h"

namespace OSRE::App {

using namespace ::OSRE::Common;
using namespace ::OSRE::Animation;
using namespace ::OSRE::RenderBackend;

DECL_OSRE_LOG_MODULE(Entity)

Entity::Entity(const String &name, Ids &ids, Scene *world) :
        Object(name),
        mRenderComponent(nullptr),
        mComponentArray(),
        mTransformNode(nullptr),
        mIds(ids),
        mAabb(),
        mOwner(world) {
    mComponentArray.resize(Component::getIndex(ComponentType::Count));
    mComponentArray.set(nullptr);
    mRenderComponent = (RenderComponent *)createComponent(ComponentType::RenderComponentType);
    if (mRenderComponent == nullptr) {
        osre_error(Tag, "Error while creating render component.");
    }
    if (nullptr != mOwner) {
        mOwner->addEntity(this);
    }
}

Entity::~Entity() {
    for (auto & i : mComponentArray) {
        delete i;
    }
    mRenderComponent = nullptr;
    if (nullptr != mOwner) {
        mOwner->removeEntity(this);
    }
}

void Entity::setNode(TransformComponent *node) {
    mTransformNode = node;
}

TransformComponent *Entity::getNode() const {
    return mTransformNode;
}

bool Entity::update(Time dt) {
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
            component = new TransformComponent(name, this, mIds, nullptr);
        } break;
        case OSRE::App::ComponentType::CameraComponentType:
            component = new CameraComponent(this);
            break;
        case OSRE::App::ComponentType::AnimationComponentType:
            component = new AnimatorComponent(this);
            break;
        case OSRE::App::ComponentType::Invalid:
        case OSRE::App::ComponentType::Count:
        default:
            break;
    }
    mComponentArray[static_cast<size_t>(type)] = component;

    return component;
}

Component *Entity::getComponent(ComponentType type) const {
    if (type == ComponentType::Count || type == ComponentType::Invalid) {
        return nullptr;
    }

    return mComponentArray[Component::getIndex(type)];
}

void Entity::setAABB(const AABB &aabb) {
    mAabb = aabb;
}

const AABB &Entity::getAABB() const {
    return mAabb;
}

} // namespace OSRE::App
