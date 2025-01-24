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
    static void releaseTransformComponent(SceneNode *child) {
        osre_assert(child != nullptr);

        child->setParent(nullptr);
        child->release();
    }
}

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Common;

static constexpr size_t NotFound = 99999999;
SceneNode::SceneNode(const String &name, Entity *owner, Ids &ids, SceneNode *parent) :
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

SceneNode::~SceneNode() {
    if (!mChildren.isEmpty()) {
        for (size_t i = 0; i < mChildren.size(); i++) {
            releaseTransformComponent(mChildren[i]);
        }
        mChildren.clear();
    }
}

void SceneNode::setParent(SceneNode *parent) {
    // weak reference
    mParent = parent;
}

SceneNode *SceneNode::getParent() const {
    return mParent;
}

SceneNode *SceneNode::createChild(const String &name) {
    SceneNode *child = new SceneNode(name, getOwner(), * mIds, this);
    mChildren.add(child);
    child->get();

    return child;
}

void SceneNode::addChild(SceneNode *child) {
    if (nullptr != child) {
        mChildren.add(child);
        child->get();
    }
}

bool SceneNode::removeChild(const String &name) {
    if (name.empty()) {
        return false;
    }

    bool found = false;
    SceneNode *currentNode = nullptr;
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

        found = currentNode->removeChild(name);
        if (found) {
            break;
        }
    }

    return found;
}

SceneNode *SceneNode::findChild(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    SceneNode *currentNode = nullptr;
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

size_t SceneNode::getNumChildren() const {
    return mChildren.size();
}

SceneNode *SceneNode::getChildAt(size_t idx) const {
    if (idx >= mChildren.size()) {
        return nullptr;
    }

    return mChildren[idx];
}

void SceneNode::releaseChildren() {
    if (mChildren.isEmpty()) {
        return;
    }

    for (ui32 i = 0; i < mChildren.size(); i++) {
        if (nullptr != mChildren[i]) {
            releaseTransformComponent(mChildren[i]);
        }
    }
}

void SceneNode::render(RenderBackendService *renderBackendSrv) {
    if (nullptr == renderBackendSrv) {
        return;
    }

    onRender(renderBackendSrv);
}

void SceneNode::setTransformationMatrix(const glm::mat4 &m) {
    mLocalTransform = m;
}

const glm::mat4 &SceneNode::getTransformationMatrix() const {
    return mLocalTransform;
}

glm::mat4 SceneNode::getWorlTransformMatrix() {
    glm::mat4 wt(1.0);
    for (const SceneNode *node = this; node != nullptr; node = node->getParent()) {
        wt *= node->getTransformationMatrix();
    }

    return wt;
}

bool SceneNode::onUpdate(Time) {
    mWorldTransform = getWorlTransformMatrix();

    return true;
}

bool SceneNode::onRender(RenderBackendService *) {
    return true;
}

void SceneNode::addMeshReference(size_t entityMeshIdx) {
    MeshReferenceArray::Iterator it = mMeshRefererenceArray.find(entityMeshIdx);
    if (mMeshRefererenceArray.end() == it) {
        mMeshRefererenceArray.add(entityMeshIdx);
    }
}

size_t SceneNode::getNumMeshReferences() const {
    return mMeshRefererenceArray.size();
}

size_t SceneNode::getMeshReferenceAt(size_t index) const {
    if (index >= mMeshRefererenceArray.size()) {
        return NotFound;
    }

    return mMeshRefererenceArray[index];
}

} // Namespace App
} // namespace OSRE
