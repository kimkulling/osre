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
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

VertComponent VertexLayout::ErrorComp;

VertComponent::VertComponent( VertexAttribute attrib, VertexFormat format )
    : m_attrib( attrib )
    , m_format( InvalidVertexFormat ) {
    // empty
}

VertComponent::~VertComponent() {
    // empty
}

VertexLayout::VertexLayout()
    : m_components()
    , m_offsets()
    , m_currentOffset( 0 ) {
    // empty
}

VertexLayout::~VertexLayout() {
    // empty
}

void VertexLayout::clear() {
    m_components.clear();
    m_offsets.clear();
    m_currentOffset = 0;
}

VertexLayout &VertexLayout::add( VertComponent *comp ) {
    if( nullptr == comp ) {
        return *this;
    }
    m_components.add( comp );
    const ui32 offset( getVertexFormatSize( comp->m_format ) );
    m_offsets.add( m_currentOffset );
    m_currentOffset += offset;

    return *this;
}

VertComponent &VertexLayout::getAt( ui32 idx ) const {
    if( idx >= m_components.size() ) {
        return ErrorComp;
    }

    return *m_components[ idx ];
}

} // Namespace RenderBackend
} // Namespace OSRE
