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
#include <osre/RenderBackend/GPUBufferManager.h>
#include "OGLRenderer/OGLRenderBackend.h"

namespace OSRE {
namespace RenderBackend {

struct OGLImpl : public GPUBufferManager::Impl {
    OGLImpl( OGLRenderBackend *glBackend )
    : Impl() 
    , m_glBackend( glBackend ) {
        OSRE_ASSERT( nullptr != m_glBackend );
    }

    virtual ~OGLImpl() {
        m_glBackend = nullptr;
    }

    Buffer *createBuffer( const String &desc ) override {
        OSRE_ASSERT( !desc.empty() );

        OGLBuffer *glBuffer( nullptr );
        if ( desc[ 0 ] == 'v' ) {
            glBuffer = m_glBackend->createBuffer( BufferType::VertexBuffer );
        }
        Buffer *buffer = new Buffer;
        buffer->m_handle = glBuffer->m_handle;
    }
    
    Buffer *getBufferByDesc( const String &desc ) override {

    }

    void updateBuffer( BufferData &data ) override {

    }

    void appendToBuffer( BufferData &data ) override {

    }

    void releaseBuffer( Buffer *buffer ) override {

    }

private:
    OGLRenderBackend *m_glBackend;
};

GPUBufferManager::GPUBufferManager( Impl *impl )
: m_bufferMap()
, m_impl( impl ) {
    // empty
}

GPUBufferManager::~GPUBufferManager() {

}

Buffer *GPUBufferManager::createBuffer( const String &desc ) {
    if ( desc.empty() ) {
        return nullptr;
    }

    m_impl->createBuffer( desc );
}

void GPUBufferManager::releaseBuffer( Buffer *buffer ) {

}

} // Namespace RenderBackend
} // Namespace OSRE
