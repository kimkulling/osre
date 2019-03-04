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
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/View.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/StringUtils.h>
#include <osre/Common/Ids.h>
#include <osre/RenderBackend/Pipeline.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

static ui32 calcHash( const String &name ) {
    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    return hash;
}

TransformBlockCache::TransformBlockCache( ui32 numIniBlocks )
: m_numBlocks( numIniBlocks )
, m_blocks( nullptr ) {
    if( 0 != m_numBlocks ) {
        m_blocks = new RenderBackend::TransformState[ m_numBlocks ];
    }
}

TransformBlockCache::~TransformBlockCache() {
    delete[] m_blocks;
    m_blocks = nullptr;
}

static void releaseChildNodes( Node *node ) {
    if( nullptr != node ) {
        node->releaseChildren();
        node->release();
        node = nullptr;
    }
}

Stage::Stage( const String &name, RenderBackendService *rbService )
: Object( name )
, m_root( nullptr )
, m_views()
, m_registeredFactories()
, m_transformBlocks( 5 )
, m_rbService( rbService )
, m_ids( nullptr ) {
    m_ids = new Ids;
    m_root = new Node( "name" + String( ".root" ), *m_ids, 
        Node::RenderCompRequest::RenderCompRequested, 
        Node::TransformCompRequest::TransformCompRequested, 
        nullptr 
    );
}

Stage::~Stage() {
    releaseChildNodes( m_root );
    m_ids = nullptr;
}

void Stage::setRoot( Node *root ) {
    m_root = root;
}

Node *Stage::getRoot() const {
    return m_root;
}

Node *Stage::createNode( const String &name, Node *parent, const String &type ) {
    if( name.empty() ) {
        return nullptr;
    }

    Node *newNode( nullptr );
    if ( "default" == type ) {
        newNode = new Node( name, 
                           *m_ids, 
                            Node::RenderCompRequest::RenderCompRequested,
                            Node::TransformCompRequest::TransformCompRequested, 
                            parent 
                        );
        if( nullptr == parent ) {
            m_root->addChild( newNode );
        }
    } else {
        const ui32 hash( calcHash( type ) );
        if ( m_registeredFactories.hasKey( hash ) ) {
            AbstractNodeFactory *factory( nullptr );
            if ( m_registeredFactories.getValue( hash, factory ) ) {
                newNode = factory->create(name, *m_ids, true, true, parent);
            }
        }
    }

    return newNode;
}

bool Stage::registerNodeFactory( AbstractNodeFactory *factory ) {
    if ( nullptr == factory ) {
        return false;
    }
    const ui32 hash( calcHash( factory->getType() ) );
    if ( m_registeredFactories.hasKey( hash ) ) {
        return false;
    }

    m_registeredFactories.insert( hash, factory );

    return true;
}

Node *Stage::findNode( const String &name ) const {
    if( nullptr == m_root || name.empty() ) {
        return nullptr;
    }

    Node *myNode( m_root->findChild( name ) );
 
    return myNode;
}

View *Stage::addView( const String &name, Node *parent ) {
    if ( name.empty() ) {
        return nullptr;
    }

    View *view( new View( name, *m_ids, parent ) );
    m_views.add( view );

    return view;
}

void Stage::clear() {
    releaseChildNodes( m_root );
}

void Stage::update( Time dt ) {
    onUpdate( dt );
}

static void renderNode( Node *current, bool traverse, RenderBackendService *rb ) {
    if (nullptr == current) {
        return;
    }

    // only update active nodes
    if (!current->isActive()) {
        return;
    }

    current->render( rb );

    // traverse all children, if requested
    if (traverse) {
        const ui32 numChilds( current->getNumChildren() );
        for (ui32 i = 0; i < numChilds; ++i) {
            Node *child( current->getChildAt( i ) );
            renderNode( child, traverse, rb );
        }
    }
}

void Stage::draw( RenderBackendService *rbSrv ) {
    if( nullptr == m_root ) {
        return;
    }

    renderNode( m_root, true, m_rbService );

    onRender( rbSrv );
}

void Stage::setIdContainer( Common::Ids &ids ) {
    m_ids = &ids;
}

Common::Ids *Stage::getIdContainer() const {
    return m_ids;
}

void Stage::onUpdate( Time dt ) {
    // empty
}

void Stage::onRender( RenderBackendService *renderBackendSrv ) {
    // empty
}

} // Namespace Scene
} // namespace OSRE
