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
