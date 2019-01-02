/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Assets/Model.h>
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

Node::Node( const String &name, Ids &ids, RenderCompRequest renderEnabled, TransformCompRequest transformEnabled, 
        Node *parent )
: Object( name )
, m_children()
, m_parent( parent )
, m_isActive( true )
, m_renderComp( nullptr )
, m_transformComp( nullptr )
, m_ids( &ids )
, m_propMap()
, m_aabb() {
    if (TransformCompRequest::TransformCompRequested == transformEnabled) {
        m_transformComp = new TransformComponent( this, m_ids->getUniqueId() );
        m_components.add( m_transformComp );
    }

	if (RenderCompRequest::RenderCompRequested == renderEnabled ) {
        m_renderComp = new RenderComponent( this, m_ids->getUniqueId() );
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

ui32 Node::getNumChildren() const {
    return m_children.size();
}

Node *Node::getChildAt( ui32 idx ) const {
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

void Node::addModel( Model *model ) {
    Node *root = model->getRootNode();
    if ( nullptr != root ) {
        addChild( root );
    }
}

void Node::addMesh( RenderBackend::Mesh *geo ) {
    if ( nullptr != m_renderComp ) {
        m_renderComp->addStaticGeometry( geo );
    }
}

ui32 Node::getNumMeshes() const {
    if ( nullptr != m_renderComp ) {
        return m_renderComp->getNumGeometry();
    }

    return 0;
}

RenderBackend::Mesh *Node::getMeshAt(ui32 idx) const {
    if ( nullptr != m_renderComp ) {
        return m_renderComp->getGeoAt( idx );
    }

    return nullptr;
}

void Node::update(Time dt) {
    onUpdate(dt);
}

void Node::draw( RenderBackendService *renderBackendSrv ) {
    if ( nullptr == renderBackendSrv || nullptr == m_renderComp ) {
        return;
    }

    onDraw( renderBackendSrv );

    // at first we need to update the transformations
    if ( nullptr != m_transformComp ) {
        m_transformComp->draw( renderBackendSrv );
    }

    // now e can render the scene
    if ( nullptr != m_renderComp ) {
        m_renderComp->draw( renderBackendSrv );
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

void Node::onUpdate(Time dt) {
    TransformComponent *comp((TransformComponent*)getComponent(Node::ComponentType::TransformComponentType));
    if (nullptr != comp) {
        comp->update(dt);
    }
}

void Node::onDraw( RenderBackendService* ) {
    // empty
}

LightNode::LightNode(const String &name, Common::Ids &ids, RenderCompRequest renderEnabled,
        TransformCompRequest transformEnabled, Node *parent)
: Node(name, ids, renderEnabled, transformEnabled, parent)
, m_light() {
    // empty
}

LightNode::~LightNode() {

}

void LightNode::setLight(const Light &light) {
    m_light = light;
}

const Light &LightNode::getLight() const {
    return m_light;
}

void LightNode::onUpdate(Time dt) {
    TransformComponent *comp( ( TransformComponent* ) getComponent( Node::ComponentType::TransformComponentType ) );
    if (nullptr != comp) {
        const TransformState &transformState(comp->getTransformState());
        glm::mat4 m(1.0f);
        transformState.toMatrix( m );
        m_light.m_position = m * m_light.m_position;
    }
}

void LightNode::onDraw( RenderBackendService *renderBackendSrv) {

}

} // Namespace Scene
} // namespace OSRE
