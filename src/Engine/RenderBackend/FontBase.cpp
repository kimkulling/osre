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
#include <osre/RenderBackend/FontBase.h>
#include <src/Engine/RenderBackend/OGLRenderer/OGLRenderBackend.h>

namespace OSRE {
namespace RenderBackend {
 
FontBase::FontBase( const String &name )
: Object( name )
, m_texName()
, m_numCols( 0 )
, m_numRows( 0 )
, m_fontAtlas( nullptr )
, m_uri() {
    // empty
}

FontBase::~FontBase() {
    // empty
}
    
void FontBase::setSize( ui32 size ) {
    m_size = size;
}
    
ui32 FontBase::getSize() const {
    return m_size;
}

void FontBase::setUri( const IO::Uri &uri ) {
    m_uri = uri;
    m_texName = m_uri.getResource();
}

void FontBase::setTextureName( const String &name ) {
    m_texName = name;
}

const String &FontBase::getTextureName() const {
    return m_texName;
}

void FontBase::setAtlasCols( ui32 numCols ) {
    m_numCols = numCols;
}

void FontBase::setAtlasRows( ui32 numRows ) {
    m_numRows = numRows;
}

bool FontBase::loadFromStream( OGLRenderBackend *rb ) {
    if ( m_uri.isEmpty() || nullptr == rb ) {
        return false;
    }

    const String texName( Object::getName() );
    if( nullptr != m_fontAtlas ) {
        rb->releaseTexture( m_fontAtlas );
        m_fontAtlas = nullptr;
    }

    m_fontAtlas = rb->createTextureFromFile( m_texName, m_uri );
    if( nullptr == m_fontAtlas ) {
        return false;
    }
    m_numRows = 16;
    m_numCols = 16;

    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
