/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Collision {


//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	
//-------------------------------------------------------------------------------------------------
template<class T>
class TQuadTree {
public:
    using Rect  = Common::TRect2D<T>;
    using Point = Common::TPoint2<T>;

    TQuadTree(const Rect& rc);
    ~TQuadTree();
    void add(const Point& pt);

private:
    struct QuadNode {
        Rect m_rect;
        QuadNode*m_children[4];
        
        QuadNode(const Rect &rc) 
        : m_rect( rc ) {
            m_children[0] = m_children[1] = m_children[2] = m_children[3] = nullptr;
        }
        
        QuadNode *isIn(const Point& pt) const {
            if (m_rect.isIn(pt)) {
                for (ui32 i = 0; i < 4; ++i) {
                    if (nullptr != m_children[i]) {
                        return m_children[i]->isIn(pt);
                    }
                }
                
                return this;
            }
        }
    };

    QuadNode*m_root;
};

template<class T>
inline
TQuadTree<T>::TQuadTree(const Rect &rc)
: m_root( nullptr ) {
    m_root = new Entity(rc);
}

template<class T>
inline
TQuadTree<T>::~TQuadTree() {
    delete m_root;
    m_root = nullptr;
}

template<class T>
inline
void TQuadTree<T>::add(const Point& pt) {

}
} // Namespace Collision
} // Namespace OSRE

