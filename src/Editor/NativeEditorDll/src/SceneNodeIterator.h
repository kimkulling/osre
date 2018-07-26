#pragma once

#include <osre/Scene/Node.h>

namespace OSRE {
namespace NativeEditor {

class SceneNodeIterator {
public:
    SceneNodeIterator(Scene::Node *root);
    ~SceneNodeIterator();
    bool hasNextChild() const;
    Scene::Node *getNextChild();
    
    SceneNodeIterator(const SceneNodeIterator &) = delete;
    SceneNodeIterator(SceneNodeIterator &) = delete;
    SceneNodeIterator &operator = ( const SceneNodeIterator & ) = delete;
        
private:
    Scene::Node *m_currentNode;
    i32 m_currentIndex;
};

inline
SceneNodeIterator::SceneNodeIterator(Scene::Node *root) 
: m_currentNode( root )
, m_currentIndex( -1 ) {
    // empty
}
        
inline
SceneNodeIterator::~SceneNodeIterator() {

}

inline
bool SceneNodeIterator::hasNextChild() const {
    if (nullptr == m_currentNode) {
        return false;
    }

    const i32 numChildren = m_currentNode->getNumChildren();
    if ((m_currentIndex + 1) < numChildren) {
        return true;
    }
    
    return false;
}

inline
Scene::Node *SceneNodeIterator::getNextChild() {
    if (hasNextChild()) {
        m_currentIndex++;
        return m_currentNode->getChildAt(m_currentIndex);
    }

    return nullptr;
}

}
}
