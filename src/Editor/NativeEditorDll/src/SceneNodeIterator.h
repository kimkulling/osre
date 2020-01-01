/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
    // empty
}

inline
bool SceneNodeIterator::hasNextChild() const {
    if (nullptr == m_currentNode) {
        return false;
    }

    const size_t numChildren = m_currentNode->getNumChildren();
    if ((m_currentIndex + 1) < numChildren) {
        return true;
    }
    
    return false;
}

inline
Scene::Node *SceneNodeIterator::getNextChild() {
    if (hasNextChild()) {
        ++m_currentIndex;
        return m_currentNode->getChildAt(m_currentIndex);
    }

    return nullptr;
}

}
}
