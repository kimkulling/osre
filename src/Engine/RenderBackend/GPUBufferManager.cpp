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
#include <osre/Common/StringUtils.h>

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
        buffer->desc = desc;
    }
    
    Buffer *getBufferByDesc( const String &desc ) override {
	return nullptr;
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
    // empty
}

Buffer *GPUBufferManager::createBuffer( const String &desc ) {
    if ( desc.empty() ) {
        return nullptr;
    }
    Buffer *buffer = getBufferByDesc( desc );
    if ( nullptr != buffer ) {
        return buffer;
    }

    buffer = m_impl->createBuffer( desc );
    if ( nullptr != buffer ) {
        const ui32 key( Common::StringUtils::hashName( desc ) );
        m_bufferMap.insert( key, buffer );
    }

    return buffer;
}

Buffer *GPUBufferManager::getBufferByDesc( const String &desc ) {
    if ( desc.empty() ) {
        return nullptr;
    }
    const ui32 key( Common::StringUtils::hashName( desc ) );
    if ( !m_bufferMap.hasKey( key ) ) {
        return nullptr;
    }
    Buffer *buffer( nullptr );
    if ( m_bufferMap.getValue( key, buffer ) ) {
        return buffer;
    }
    
    return nullptr;
}

void GPUBufferManager::releaseBuffer( Buffer *buffer ) {
    if ( nullptr == buffer ) {
        return;
    }
    m_impl->releaseBuffer( buffer );
    const ui32 key( Common::StringUtils::hashName( buffer->desc ) );
    m_bufferMap.remove( key );
}

} // Namespace RenderBackend
} // Namespace OSRE

