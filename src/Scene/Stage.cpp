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
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/View.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace Scene {

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
, m_transformBlocks( 5 )
, m_rbService( rbService ) {
    // empty
}

Stage::~Stage() {
    releaseChildNodes( m_root );
}

void Stage::setRoot( Node *root ) {
    m_root = root;
}

Node *Stage::getRoot() const {
    return m_root;
}

Node *Stage::addNode( const String &name, Node *parent ) {
    if( name.empty() ) {
        return nullptr;
    }

    Node *newNode = new Node( name, true, parent );
    if( nullptr == parent ) {
        m_root = newNode;
        m_root->get();
    }

    return newNode;
}

Node *Stage::findNode( const String &name ) const {
    if( nullptr == m_root || name.empty() ) {
        return nullptr;
    }

    Node *myNode( m_root->findChild( name ) );
    return myNode;
}

View *Stage::addView( const String &name, Node *node ) {
    View *view = new View( name );
    m_views.add( view );

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

void Stage::update() {
    if( nullptr == m_root ) {
        return;
    }

    updateNode( m_root, true, m_rbService );
}

} // Namespace Scene
} // namespace OSRE
