/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/Component.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/Ids.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

Node::Node( const String &name, Common::Ids &ids, bool transformEnabled, bool renderEnabled, Node *parent )
: Object( name )
, m_childs()
, m_parent( parent )
, m_isActive( true )
, m_localTransform( nullptr )
, m_renderComp( nullptr )
, m_transformComp( nullptr )
, m_ids( &ids ) {
    if ( transformEnabled ) {
        m_transformComp = new TransformComponent( m_ids->getUniqueId() );
        m_components.add( m_transformComp );
    }
    if ( renderEnabled ) {
        m_renderComp = new RenderComponent( m_ids->getUniqueId() );
        m_components.add( m_renderComp );
    }

    if ( nullptr != m_parent ) {
        m_parent->addChild( this );
    }
}

Node::~Node() {
    for ( ui32 i = 0; i < m_components.size(); i++ ) {
        delete m_components[ i ];
    }
    m_components.clear();

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

        if( TraverseMode::RecursiveMode == mode ) {
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
    if ( nullptr != m_renderComp ) {
        m_renderComp->addStaticGeometry( geo );
    }
}

void Node::update( RenderBackend::RenderBackendService *renderBackendSrv ) {
    // at first we need to update the transformations
    if ( nullptr != m_transformComp ) {
        m_transformComp->update( renderBackendSrv );
    }

    // now e can render the scene
    if ( nullptr != m_renderComp ) {
        m_renderComp->update( renderBackendSrv );
    }
}

Component *Node::getComponent( ComponentType type ) const {
    if ( ComponentType::RenderComponentType == type ) {
        return m_renderComp;
    } else if ( ComponentType::TransformComponentType == type ) {
        return m_transformComp;
    }

    return nullptr;
}

void Node::setActive( bool isActive ) {
    m_isActive = isActive;
}

bool Node::isActive() const {
    return m_isActive;
}

} // Namespace Scene
} // namespace OSRE
