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

class OSRE_EXPORT Stage : public Common::Object {
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
