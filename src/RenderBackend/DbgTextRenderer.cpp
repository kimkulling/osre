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
#include <osre/RenderBackend/DbgTextRenderer.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <vector>

namespace OSRE {
namespace RenderBackend {

TextRenderer2D::TextRenderer2D()
: m_data( nullptr )
, m_text()
, m_dirty( false )
, m_xCursor( -1 )
, m_yCursor( -1 )
, m_sx( -1 )
, m_sy( -1 ) {
	// empty
}

TextRenderer2D::~TextRenderer2D() {
	clear();
}

void TextRenderer2D::setCursor( ui32  x, ui32 y ) {
	m_xCursor = x;
	m_yCursor = y;
}

void TextRenderer2D::setScale( i32 sx, i32 sy ) {
	m_sx = sx;
	m_sy = sy;
}

void TextRenderer2D::addText( const String &text ) {
    m_text += text;
	m_dirty = true;
}

void TextRenderer2D::clear() {
	m_text.clear();
	BufferData::free(m_data);
	m_data = nullptr;
}

void TextRenderer2D::drawText() {
    if( m_text.empty() ) {
        return;
    }

	if (m_dirty) {
        // TODO!
	}
}

} // Namespace RenderBackend
} // Namespace OSRE
