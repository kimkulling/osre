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
#include <osre/RenderBackend/HWBufferManager.h>
#include <osre/Common/StringUtils.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

Buffer::Buffer()
: m_id( 0 )
, m_ptr( nullptr ) {
    // empty
}

HWBufferManager::HWBufferManager()
: m_buffers()
, m_bufferDict () {
    // empty
}

HWBufferManager::~HWBufferManager() {
    // empty
}

static c8 *getVertexCompShortCut( VertexAttribute &attrib ) {
    switch (attrib) {
        case VertexAttribute::Position:
            return "p";
        case VertexAttribute::Normal:
            return "n";
        case VertexAttribute::TexCoord0:
            return "t0";
        case VertexAttribute::TexCoord1:
            return "t1";
        case VertexAttribute::TexCoord2:
            return "t2";
        case VertexAttribute::TexCoord3:
            return "t3";
        case VertexAttribute::Tangent:
            return "ta";
        case VertexAttribute::Binormal:
            return "bt";
        case VertexAttribute::Weights:
            return "w";
        case VertexAttribute::Indices:
            return "i";
        case VertexAttribute::Color0:
            return "c0";
        case VertexAttribute::Color1:
            return "c1";
        case VertexAttribute::Instance0:
            return "i0";
        case VertexAttribute::Instance1:
            return "i1";
        case VertexAttribute::Instance2:
            return "i2";
        case VertexAttribute::Instance3:
            return "i3";
        default:
            break;
    }

    return nullptr;
}

static c8 *getAccessShortCut( BufferAccessType access ) {
    switch (access) {
        case BufferAccessType::ReadOnly:
            return "r";
        case BufferAccessType::WriteOnly:
            return "w";
        case BufferAccessType::ReadWrite:
            return "rw";
        default:
            break;
    }
    return nullptr;
}

void getShortcut( const VertexLayout &vertexLayout, BufferAccessType access, ui32 &hash ) {
    String key;
    for (ui32 i = 0; i < vertexLayout.numComponents(); ++i) {
        VertComponent &vc = vertexLayout.getAt( i );
        key += getVertexCompShortCut( vc.m_attrib );
    }
    key += getAccessShortCut( access );
    hash = StringUtils::hashName( key );

}
Buffer *HWBufferManager::createBuffer( const VertexLayout &vertexLayout, BufferAccessType access ) {
    Buffer *buffer = new Buffer();
    
    ui32 hash;
    getShortcut( vertexLayout, access, hash );
    m_bufferDict.insert(hash, buffer );
    ui32 id = m_buffers.size();
    m_buffers.add( buffer );
    buffer->m_id = id;

    return buffer;
}

Buffer *HWBufferManager::getBufferByDesc( VertexLayout vertexLayout, BufferAccessType access ) {
    if ( m_buffers.isEmpty()) {
        return nullptr;
    }
    
    ui32 hash;
    getShortcut( vertexLayout, access, hash );
    Buffer *buffer( nullptr );
    if (m_bufferDict.hasKey( hash )) {
        m_bufferDict.getValue( hash, buffer );
    }

    return buffer;
}

void HWBufferManager::clear() {
    for (ui32 i = 0; i < m_buffers.size(); ++i) {
        delete m_buffers[ i ];
    }
    m_buffers.clear();
    m_bufferDict.clear();
}

}
}
