/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Assets/Model.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::Common;

static ui32 calcHash( const String &name ) {
    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    return hash;
}

TransformBlockCache::TransformBlockCache( ui32 numIniBlocks )
: m_numBlocks( numIniBlocks )
, m_blocks( nullptr ) {
    if( 0 != m_numBlocks ) {
        m_blocks = new RenderBackend::TransformBlock[ m_numBlocks ];
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

Stage::Stage( const String &name, RenderBackend::RenderBackendService *rbService )
: Object( name )
, m_root( nullptr )
, m_views()
, m_registeredFactories()
, m_transformBlocks( 5 )
, m_rbService( rbService )
, m_ids( nullptr ) {
    m_ids = new Ids;
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

Node *Stage::addNode( const String &name, Node *parent, const String &type ) {
    if( name.empty() ) {
        return nullptr;
    }

    Node *newNode( nullptr );
    if ( "default" == type ) {
        newNode = new Node( name, *m_ids, true, true, parent );
        if( nullptr == parent ) {
            m_root = newNode;
            m_root->get();
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

View *Stage::addView( const String &name, Node *node ) {
    if ( name.empty() ) {
        return nullptr;
    }

    View *view( new View( name ) );
    m_views.add( view );
    if ( nullptr != node ) {
        view->observeNode( node );
    }

    return view;
}

void Stage::clear() {
    releaseChildNodes( m_root );
}

static void updateNode( Node *current, bool traverse, RenderBackend::RenderBackendService *rb ) {
    if ( nullptr == current ) { 
        return;
    }
    
    // only update active nodes
    if ( !current->isActive() ) {
        return;
    }

    current->update( rb );
 
    // traverse all children, if requested
    if ( traverse ) {
        const ui32 numChilds( current->getNumChilds() );
        for ( ui32 i = 0; i < numChilds; ++i ) {
            Node *child( current->getChildAt( i ) );
            updateNode( child, traverse, rb );
        }
    }
}

void Stage::update( RenderBackend::RenderBackendService *renderBackendSrv ) {
    if( nullptr == m_root ) {
        return;
    }

    updateNode( m_root, true, m_rbService );
}

void Stage::setIdContainer( Common::Ids &ids ) {
    m_ids = &ids;
}

Common::Ids *Stage::getIdContainer() const {
    return m_ids;
}

} // Namespace Scene
} // namespace OSRE
