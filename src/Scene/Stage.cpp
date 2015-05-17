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
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/RenderBackend/RenderCommon.h>

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

Stage::Stage( const String &name )
    : Object( name )
    , m_root( nullptr )
    , m_transformBlocks( 5 ) {
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

    Node *newNode = new Node( name, parent );
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

void Stage::clear() {
    releaseChildNodes( m_root );
}

} // Namespace Scene
} // namespace OSRE
