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
#include <osre/UI/TextBase.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/RenderBackend/FontBase.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

TextBase::TextBase( const String &name, Widget *parent )
: Widget( name, parent )
, m_font( nullptr )
, m_text( "" )
, m_textGlyphes() {
    // empty
}
    
TextBase::~TextBase() {
    // empty
}

void TextBase::setLabel( const String &text ) {
    if ( text == m_text ) {
        return;
    }
    
    m_text = text;
    Widget::requestRedraw();
}

const String &TextBase::getLabel() const {
    return m_text;
}

void TextBase::setFont( RenderBackend::FontBase *font ) {
    if ( m_font == font ) {
        return;
    }
    
    m_font = font;
    Widget::requestRedraw();
}

RenderBackend::FontBase *TextBase::getFont() const {
    return m_font;
}

void TextBase::onRender( TargetGeoArray &targetGeoArray, RenderBackendService *rbSrv ) {
    if ( m_text.empty() ) {
        return;
    }
    f32 fontSize = 0.1f;
    if ( nullptr != m_font ) {
        fontSize = m_font->getSize();
    }
    if ( nullptr == m_textGlyphes ) {
        m_textGlyphes = GeometryBuilder::allocTextBox( 0, 0, fontSize, m_text, BufferAccessType::ReadWrite );
    }
    targetGeoArray.add( m_textGlyphes );
}

} // Namespace UI
} // Namespace OSRE
