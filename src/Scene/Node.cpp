/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/Node.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace Scene {

static const glm::vec4 Dummy;

using namespace RenderBackend;

Node::Node( const String &name, Node *parent )
: Object( name )
, m_childs()
, m_newGeo()
, m_parent( parent )
, m_transform( nullptr ) {
    // empty
}

Node::~Node() {
    if( !m_childs.isEmpty() ) {
        for( size_t i = 0; i < m_childs.size(); i++ ) {
            m_childs[ i ]->release();
        }
        m_childs.clear();
    }
}
    
void Node::setParent( Node *parent ) {
    m_parent = parent;
}
    
Node *Node::getParent() const {
    return m_parent;
}

void Node::addChild( Node *child ) {
    if( nullptr == child ) {
        return;
    }

    m_childs.add( child );
    child->setParent( this );
    child->get();
}

bool Node::removeChild( const String &name, TraverseMode mode ) {
    if( name.empty() ) {
        return false;
    }

    bool found( false );
    Node *currentNode( nullptr );
    for( ui32 i = 0; i < m_childs.size(); i++ ) {
        currentNode = m_childs[ i ];
        if( nullptr != currentNode ) {
            if( currentNode->getName() == name ) {
                found = true;
                m_childs.remove( i );
                currentNode->release();
                break;
            }
        }

        if( RecursiveMode == mode ) {
            found = currentNode->removeChild( name, mode );
            if( found ) {
                break;
            }
        }
    }

    return found;
}

Node *Node::findChild( const String &name ) const {
    if( name.empty() ) {
        return nullptr;
    }

    Node *currentNode( nullptr );
    for( ui32 i = 0; i < m_childs.size(); i++ ) {
        currentNode = m_childs[ i ];
        if( nullptr != currentNode ) {
            if( currentNode->getName() == name ) {
                return currentNode;
            }
        }
    }

    return nullptr;
}

ui32 Node::getNumChilds() const {
    return m_childs.size();
}

Node *Node::getChildAt( ui32 idx ) const {
    if( idx >= m_childs.size() ) {
        return nullptr;
    }

    return m_childs[ idx ];
}

void Node::releaseChildren() {
    if( m_childs.isEmpty() ) {
        return;
    }

    for( ui32 i = 0; i < m_childs.size(); i++ ) {
        if( nullptr != m_childs[ i ] ) {
            m_childs[ i ]->releaseChildren();
            m_childs[ i ]->release();
        }
    }
}

void Node::addGeometry( RenderBackend::Geometry *geo ) {
    m_newGeo.add( geo );
}

void Node::setTransformBlock( RenderBackend::TransformBlock *transformBlock ) {
    if( nullptr != transformBlock ) {
        m_transform = transformBlock;
    }
}

void Node::setPosition( const glm::vec3 &pos ) {
    if( nullptr != m_transform ) {
        m_transform->m_transform = glm::vec4( pos, 1.0f );
    }
}

const glm::vec4 &Node::getPosition() const {
    if( nullptr == m_transform ) {
        return Dummy;
    }

    return m_transform->m_transform;
}

void Node::setScale( const glm::vec3 &scale ) {
    if( nullptr != m_transform ) {
        m_transform->m_scale = glm::vec4( scale, 1.0f );;
    }
}

const glm::vec4 &Node::getScale() const {
    if( nullptr == m_transform ) {
        return Dummy;
    }

    return m_transform->m_scale;
}

void Node::update( RenderBackend::RenderBackendService *renderBackendSrv ) {
    if( !m_newGeo.isEmpty() ) {
        renderBackendSrv->sendEvent( &OnAttachViewEvent, nullptr );
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;
        attachGeoEvData->m_numGeo = m_newGeo.size();
        attachGeoEvData->m_geo = m_newGeo[ 0 ];
        renderBackendSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );
		m_newGeo.resize( 0 );
    }
}

} // Namespace Scene
} // namespace OSRE
