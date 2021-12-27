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
#include <osre/App/Component.h>
#include <osre/Common/Ids.h>
#include <osre/Common/StringUtils.h>
#include <osre/Properties/Property.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/Node.h>
#include <osre/Common/glm_common.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Common;

Node::Node(const String &name, Ids &ids, Node *parent) :
        Object(name),
        m_children(),
        m_parent(parent),
        m_meshRefererenceArray(),
        m_isActive(true),
        m_ids(&ids),
        mPropertyArray(),
        m_propMap(),
        m_localTransform() {
    if (nullptr != m_parent) {
        m_parent->addChild(this);
    }
}

Node::~Node() {
    if (!m_children.isEmpty()) {
        for (size_t i = 0; i < m_children.size(); i++) {
            m_children[i]->setParent(nullptr);
            m_children[i]->release();
        }
        m_children.clear();
    }
}

void Node::setParent(Node *parent) {
    // weak reference
    m_parent = parent;
}

Node *Node::getParent() const {
    return m_parent;
}

Node *Node::createChild(const String &name) {
    Node *child = new Node(name, *m_ids, this);
    m_children.add(child);
    child->get();

    return child;
}

void Node::addChild(Node *child) {
    if (nullptr != child) {
        m_children.add(child);
        child->get();
    }
}

bool Node::removeChild(const String &name, TraverseMode mode) {
    if (name.empty()) {
        return false;
    }

    bool found(false);
    Node *currentNode(nullptr);
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

Node *Node::findChild(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    Node *currentNode(nullptr);
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

size_t Node::getNumChildren() const {
    return m_children.size();
}

Node *Node::getChildAt(size_t idx) const {
    if (idx >= m_children.size()) {
        return nullptr;
    }

    return m_children[idx];
}

void Node::releaseChildren() {
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

void Node::update(Time dt) {
    onUpdate(dt);
}

void Node::render(RenderBackendService *renderBackendSrv) {
    if (nullptr == renderBackendSrv) {
        return;
    }

    onRender(renderBackendSrv);
}

void Node::setProperty(Properties::Property *prop) {
    if (prop == nullptr) {
        return;
    }

    const ui32 hashId = StringUtils::hashName(prop->getPropertyName().c_str());
    m_propMap.insert(hashId, prop);
    mPropertyArray.add(prop);
}

void Node::getPropertyArray( ::CPPCore::TArray<Properties::Property *> &propArray ) {
    propArray = mPropertyArray;
}

Properties::Property *Node::getProperty(const String name) const {
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

void Node::translate(const glm::vec3 &pos) {
    m_localTransform = glm::translate(m_localTransform, pos);
}

void Node::scale(const glm::vec3 &scale) {
    m_localTransform = glm::scale(m_localTransform, scale);
}

void Node::rotate(f32 angle, const glm::vec3 &axis) {
    m_localTransform = glm::rotate(m_localTransform, angle, axis);
}

void Node::setRotation(glm::quat &rotation) {
    glm::toMat4(rotation);
}

void Node::setTransformationMatrix(const glm::mat4 &m) {
    m_localTransform = m;
}

const glm::mat4 &Node::getTransformationMatrix() const {
    return m_localTransform;
}

glm::mat4 Node::getWorlTransformMatrix() {
    glm::mat4 wt(1.0);
    for (const Node *node = this; node != nullptr; node = node->getParent()) {
        wt = node->getTransformationMatrix() * wt;
    }

    return wt;
}

void Node::onUpdate(Time dt) {
    // empty
}

void Node::onRender(RenderBackendService *) {
    // empty
}

void Node::addMeshReference(size_t entityMeshIdx) {
    MeshReferenceArray::Iterator it = m_meshRefererenceArray.find(entityMeshIdx);
    if (m_meshRefererenceArray.end() == it) {
        m_meshRefererenceArray.add(entityMeshIdx);
    }
}

size_t Node::getNumMeshReferences() const {
    return m_meshRefererenceArray.size();
}

size_t Node::getMeshReferenceAt(size_t index) const {
    if (index >= m_meshRefererenceArray.size()) {
        return 99999999;
    }

    return m_meshRefererenceArray[index];
}

} // Namespace Scene
} // namespace OSRE
