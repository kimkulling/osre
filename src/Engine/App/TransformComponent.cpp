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
#include "App/Component.h"
#include "App/TransformComponent.h"
#include "Common/Ids.h"
#include "Common/StringUtils.h"
#include "Common/glm_common.h"
#include "Properties/Property.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"

namespace OSRE {
namespace App {

namespace {
    static void releaseTransformComponent(TransformComponent *child) {
        osre_assert(child != nullptr);

        child->setParent(nullptr);
        child->release();
    }
}

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Common;

static constexpr size_t NotFound = 99999999;
TransformComponent::TransformComponent(const String &name, Entity *owner, Ids &ids, TransformComponent *parent) :
        Object(name),
        Component(owner, ComponentType::TransformComponentType),
        mChildren(),
        mParent(parent),
        mMeshRefererenceArray(),
        mIsActive(true),
        mIds(&ids),
        mLocalTransform(1.0f),
        mWorldTransform(1.0f)  {
    if (nullptr != mParent) {
        mParent->addChild(this);
    }
}

TransformComponent::~TransformComponent() {
    if (!mChildren.isEmpty()) {
        for (size_t i = 0; i < mChildren.size(); i++) {
            releaseTransformComponent(mChildren[i]);
        }
        mChildren.clear();
    }
}

void TransformComponent::setParent(TransformComponent *parent) {
    // weak reference
    mParent = parent;
}

TransformComponent *TransformComponent::getParent() const {
    return mParent;
}

TransformComponent *TransformComponent::createChild(const String &name) {
    TransformComponent *child = new TransformComponent(name, getOwner(), * mIds, this);
    mChildren.add(child);
    child->get();

    return child;
}

void TransformComponent::addChild(TransformComponent *child) {
    if (nullptr != child) {
        mChildren.add(child);
        child->get();
    }
}

bool TransformComponent::removeChild(const String &name, TraverseMode mode) {
    if (name.empty()) {
        return false;
    }

    bool found = false;
    TransformComponent *currentNode = nullptr;
    for (ui32 i = 0; i < mChildren.size(); i++) {
        currentNode = mChildren[i];
        if (nullptr != currentNode) {
            if (currentNode->getName() == name) {
                found = true;
                mChildren.remove(i);
                releaseTransformComponent(currentNode);
                break;
            }
        }

        if (TraverseMode::RecursiveMode == mode) {
            found = currentNode->removeChild(name, mode);
            if (found) {
                break;
            }
        }
    }

    return found;
}

TransformComponent *TransformComponent::findChild(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    TransformComponent *currentNode = nullptr;
    for (ui32 i = 0; i < mChildren.size(); i++) {
        currentNode = mChildren[i];
        if (nullptr != currentNode) {
            if (currentNode->getName() == name) {
                return currentNode;
            }
        }
    }

    return nullptr;
}

size_t TransformComponent::getNumChildren() const {
    return mChildren.size();
}

TransformComponent *TransformComponent::getChildAt(size_t idx) const {
    if (idx >= mChildren.size()) {
        return nullptr;
    }

    return mChildren[idx];
}

void TransformComponent::releaseChildren() {
    if (mChildren.isEmpty()) {
        return;
    }

    for (ui32 i = 0; i < mChildren.size(); i++) {
        if (nullptr != mChildren[i]) {
            releaseTransformComponent(mChildren[i]);
        }
    }
}

void TransformComponent::render(RenderBackendService *renderBackendSrv) {
    if (nullptr == renderBackendSrv) {
        return;
    }

    onRender(renderBackendSrv);
}

void TransformComponent::translate(const glm::vec3 &pos) {
    mLocalTransform = glm::translate(mLocalTransform, pos);
}

void TransformComponent::scale(const glm::vec3 &scale) {
    mLocalTransform = glm::scale(mLocalTransform, scale);
}

void TransformComponent::rotate(f32 angle, const glm::vec3 &axis) {
    mLocalTransform = glm::rotate(mLocalTransform, angle, axis);
}

void TransformComponent::setRotation(glm::quat &rotation) {
    glm::toMat4(rotation);
}

void TransformComponent::setTransformationMatrix(const glm::mat4 &m) {
    mLocalTransform = m;
}

const glm::mat4 &TransformComponent::getTransformationMatrix() const {
    return mLocalTransform;
}

glm::mat4 TransformComponent::getWorlTransformMatrix() {
    glm::mat4 wt(1.0);
    for (const TransformComponent *node = this; node != nullptr; node = node->getParent()) {
        wt *= node->getTransformationMatrix();
    }

    return wt;
}

bool TransformComponent::onUpdate(Time) {
    mWorldTransform = getWorlTransformMatrix();

    return true;
}

bool TransformComponent::onRender(RenderBackendService *) {
    return true;
}

void TransformComponent::addMeshReference(size_t entityMeshIdx) {
    MeshReferenceArray::Iterator it = mMeshRefererenceArray.linearSearch(entityMeshIdx);
    if (mMeshRefererenceArray.end() == it) {
        mMeshRefererenceArray.add(entityMeshIdx);
    }
}

size_t TransformComponent::getNumMeshReferences() const {
    return mMeshRefererenceArray.size();
}

size_t TransformComponent::getMeshReferenceAt(size_t index) const {
    if (index >= mMeshRefererenceArray.size()) {
        return NotFound;
    }

    return mMeshRefererenceArray[index];
}

} // Namespace App
} // namespace OSRE
