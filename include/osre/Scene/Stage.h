#pragma once

#include <osre/Common/Object.h>

namespace OSRE {

namespace RenderBackend {
    struct TransformBlock;
}

namespace Scene {

class Node;

struct TransformBlockCache {
    ui32 m_numBlocks;
    RenderBackend::TransformBlock *m_blocks;

    TransformBlockCache( ui32 numIniBlocks );
    ~TransformBlockCache();

private:
    TransformBlockCache();
    TransformBlockCache( const TransformBlockCache & );
    TransformBlockCache &operator = ( const TransformBlockCache & );
};

class Stage : public Common::Object {
public:
    Stage( const String &name );
    virtual ~Stage();
    virtual void setRoot( Node *root );
    virtual Node *getRoot() const;
    virtual Node *addNode( const String &name, Node *parent );
    virtual Node *findNode( const String &name ) const;
    virtual void clear();
    
private:
    Node *m_root;
    TransformBlockCache m_transformBlocks;
};

} // Namespace Scene
} // namespace OSRE
