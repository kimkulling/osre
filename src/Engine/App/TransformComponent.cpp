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
#include <osre/App/Component.h>
#include <osre/Common/Ids.h>
#include <osre/Common/StringUtils.h>
#include <osre/Properties/Property.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/App/TransformComponent.h>
#include <osre/Common/glm_common.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Common;

TransformComponent::TransformComponent(const String &name, Entity *owner, Ids &ids, TransformComponent *parent) :
        Object(name),
        Component(owner, ComponentType::TransformComponentType),
        m_children(),
        m_parent(parent),
        m_meshRefererenceArray(),
        m_isActive(true),
        m_ids(&ids),
        mPropertyArray(),
        m_propMap(),
        m_localTransform(1.0f) {
    if (nullptr != m_parent) {
        m_parent->addChild(this);
    }
}

TransformComponent::~TransformComponent() {
    if (!m_children.isEmpty()) {
        for (size_t i = 0; i < m_children.size(); i++) {
            m_children[i]->setParent(nullptr);
            m_children[i]->release();
        }
        m_children.clear();
    }
}

void TransformComponent::setParent(TransformComponent *parent) {
    // weak reference
    m_parent = parent;
}

TransformComponent *TransformComponent::getParent() const {
    return m_parent;
}

TransformComponent *TransformComponent::createChild(const String &name) {
    TransformComponent *child = new TransformComponent(name, getOwner(), * m_ids, this);
    m_children.add(child);
    child->get();

    return child;
}

void TransformComponent::addChild(TransformComponent *child) {
    if (nullptr != child) {
        m_children.add(child);
        child->get();
    }
}

bool TransformComponent::removeChild(const String &name, TraverseMode mode) {
    if (name.empty()) {
        return false;
    }

    bool found(false);
    TransformComponent *currentNode(nullptr);
    for (ui32 i = 0; i < m_children.size(); i++) {
        currentNode = m_children[i];
        if (nullptr != currentNode) {
            if (currentNode->getName() == name) {
                found = true;
                m_children.remove(i);
                currentNode->release();
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

    TransformComponent *currentNode(nullptr);
    for (ui32 i = 0; i < m_children.size(); i++) {
        currentNode = m_children[i];
        if (nullptr != currentNode) {
            if (currentNode->getName() == name) {
                return currentNode;
            }
        }
    }

    return nullptr;
}

size_t TransformComponent::getNumChildren() const {
    return m_children.size();
}

TransformComponent *TransformComponent::getChildAt(size_t idx) const {
    if (idx >= m_children.size()) {
        return nullptr;
    }

    return m_children[idx];
}

void TransformComponent::releaseChildren() {
    if (m_children.isEmpty()) {
        return;
    }

    for (ui32 i = 0; i < m_children.size(); i++) {
        if (nullptr != m_children[i]) {
            m_children[i]->releaseChildren();
            m_children[i]->release();
        }
    }
}

void TransformComponent::update(Time dt) {
    onUpdate(dt);
}

void TransformComponent::render(RenderBackendService *renderBackendSrv) {
    if (nullptr == renderBackendSrv) {
        return;
    }

    onRender(renderBackendSrv);
}

void TransformComponent::setProperty(Properties::Property *prop) {
    if (prop == nullptr) {
        return;
    }

    const HashId hashId = StringUtils::hashName(prop->getPropertyName().c_str());
    m_propMap.insert(hashId, prop);
    mPropertyArray.add(prop);
}

void TransformComponent::getPropertyArray( ::cppcore::TArray<Properties::Property *> &propArray ) {
    propArray = mPropertyArray;
}

Properties::Property *TransformComponent::getProperty(const String name) const {
    const ui32 hashId = StringUtils::hashName(name.c_str());
    if (!m_propMap.hasKey(hashId)) {
        return nullptr;
    }

    Properties::Property *prop = nullptr;
    if (m_propMap.getValue(hashId, prop)) {
        return prop;
    }

    return nullptr;
}

void TransformComponent::translate(const glm::vec3 &pos) {
    m_localTransform = glm::translate(m_localTransform, pos);
}

void TransformComponent::scale(const glm::vec3 &scale) {
    m_localTransform = glm::scale(m_localTransform, scale);
}

void TransformComponent::rotate(f32 angle, const glm::vec3 &axis) {
    m_localTransform = glm::rotate(m_localTransform, angle, axis);
}

void TransformComponent::setRotation(glm::quat &rotation) {
    glm::toMat4(rotation);
}

void TransformComponent::setTransformationMatrix(const glm::mat4 &m) {
    m_localTransform = m;
}

const glm::mat4 &TransformComponent::getTransformationMatrix() const {
    return m_localTransform;
}

glm::mat4 TransformComponent::getWorlTransformMatrix() {
    glm::mat4 wt(1.0);
    for (const TransformComponent *node = this; node != nullptr; node = node->getParent()) {
        wt *= node->getTransformationMatrix();
    }

    return wt;
}

bool TransformComponent::onPreprocess() {
    return true;
}

bool TransformComponent::onUpdate(Time) {
    return true;
}

bool TransformComponent::onRender(RenderBackendService *) {
    return true;
}

bool TransformComponent::onPostprocess() {
    return true;
}

void TransformComponent::addMeshReference(size_t entityMeshIdx) {
    MeshReferenceArray::Iterator it = m_meshRefererenceArray.find(entityMeshIdx);
    if (m_meshRefererenceArray.end() == it) {
        m_meshRefererenceArray.add(entityMeshIdx);
    }
}

size_t TransformComponent::getNumMeshReferences() const {
    return m_meshRefererenceArray.size();
}

size_t TransformComponent::getMeshReferenceAt(size_t index) const {
    if (index >= m_meshRefererenceArray.size()) {
        return 99999999;
    }

    return m_meshRefererenceArray[index];
}

} // Namespace App
} // namespace OSRE
