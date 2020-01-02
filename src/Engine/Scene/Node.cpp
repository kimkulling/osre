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
#include <osre/Scene/Node.h>
#include <osre/App/Component.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/Ids.h>
#include <osre/Common/StringUtils.h>
#include <osre/Properties/Property.h>

#include <glm/gtc/matrix_transform.hpp>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Common;
using namespace ::OSRE::Assets;

Node::Node( const String &name, Ids &ids, Node *parent )
: Object( name )
, m_children()
, m_parent( parent )
, m_isActive( true )
, m_ids( &ids )
, m_propMap()
, m_aabb() {
    if ( nullptr != m_parent ) {
        m_parent->addChild( this );
    }
}

Node::~Node() {
    if( !m_children.isEmpty() ) {
        for( size_t i = 0; i < m_children.size(); i++ ) {
            m_children[ i ]->setParent(nullptr);
            m_children[ i ]->release();
        }
        m_children.clear();
    }
}
    
void Node::setParent( Node *parent ) {
    // weak reference
    m_parent = parent;
}
    
Node *Node::getParent() const {
    return m_parent;
}

void Node::addChild( Node *child ) {
    if( nullptr == child ) {
        return;
    }

    m_children.add( child );
    child->get();
}

bool Node::removeChild( const String &name, TraverseMode mode ) {
    if( name.empty() ) {
        return false;
    }

    bool found( false );
    Node *currentNode( nullptr );
    for( ui32 i = 0; i < m_children.size(); i++ ) {
        currentNode = m_children[ i ];
        if( nullptr != currentNode ) {
            if( currentNode->getName() == name ) {
                found = true;
                m_children.remove( i );
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
    for( ui32 i = 0; i < m_children.size(); i++ ) {
        currentNode = m_children[ i ];
        if( nullptr != currentNode ) {
            if( currentNode->getName() == name ) {
                return currentNode;
            }
        }
    }

    return nullptr;
}

size_t Node::getNumChildren() const {
    return m_children.size();
}

Node *Node::getChildAt( size_t idx ) const {
    if( idx >= m_children.size() ) {
        return nullptr;
    }

    return m_children[ idx ];
}

void Node::releaseChildren() {
    if( m_children.isEmpty() ) {
        return;
    }

    for( ui32 i = 0; i < m_children.size(); i++ ) {
        if( nullptr != m_children[ i ] ) {
            m_children[ i ]->releaseChildren();
            m_children[ i ]->release();
        }
    }
}

void Node::update(Time dt) {
    onUpdate(dt);
}

void Node::render( RenderBackendService *renderBackendSrv ) {
    if ( nullptr == renderBackendSrv ) {
        return;
    }

    onRender( renderBackendSrv );
}

void Node::setProperty( Properties::Property *prop) {
    const ui32 hashId( StringUtils::hashName( prop->getPropertyName().c_str() ) );
    m_propMap.insert( hashId, prop );
}

Properties::Property *Node::getProperty(const String name) const {
    const ui32 hashId( StringUtils::hashName( name.c_str() ) );
    if (!m_propMap.hasKey( hashId ) ) {
        return nullptr;
    }

    Properties::Property *prop(nullptr);
    if (m_propMap.getValue(hashId, prop )) {
        return prop;
    }

    return nullptr;
}

void Node::setTranslation( const glm::vec3 &pos ) {
    m_localTransformState.m_translate = glm::vec3( pos );
    m_dirty = NeedsTransform;
}

const glm::vec3 &Node::getTranslation() const {
    return m_localTransformState.m_translate;
}

void Node::setScale( const glm::vec3 &scale ) {
    m_localTransformState.m_scale = glm::vec3( scale );;
    m_dirty = NeedsTransform;
}

const glm::vec3 &Node::getScale() const {
    return m_localTransformState.m_scale;
}

void Node::setTransformationMatrix( const glm::mat4 &m ) {
    m_transform = m;
}

const glm::mat4 &Node::getTransformationMatrix() const {
    return m_transform;
}

glm::mat4 Node::getWorlTransformMatrix() {
    glm::mat4 wt( 1.0 );
    for (const Node *node = this; node != nullptr; node = node->getParent()) {
        wt = node->getTransformationMatrix() * wt;
    }

    return wt;
}

const RenderBackend::TransformState &Node::getTransformState() const {
    return m_localTransformState;
}

void Node::onUpdate(Time dt) {
    if (m_dirty == NeedsTransform) {
        m_localTransformState.toMatrix( m_transform );
        m_dirty = NotDirty;
    }
}

void Node::onRender( RenderBackendService* ) {
    // empty
}

} // Namespace Scene
} // namespace OSRE
