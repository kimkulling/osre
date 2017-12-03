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
#include <osre/Common/Tokenizer.h>
#include <osre/RenderBackend/FontBase.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
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

static const ui32 NumQuadVert = 4;

static ui32 getNumTextVerts( const String &text ) {
    const ui32 NumTextVerts = NumQuadVert * text.size();
    return NumTextVerts;
}

void TextBase::onRender( UiRenderCmdCache &renderCmdCache, RenderBackendService *rbSrv ) {
    if ( m_text.empty() ) {
        return;
    }
    f32 fontSize = 0.1f;
    if ( nullptr != m_font ) {
        fontSize = static_cast<f32>( m_font->getSize() );
    }

    RenderBackend::RenderVert *verts;
    if ( nullptr == m_textGlyphes ) {
        f32 x( Widget::getRect().getX1() );
        f32 y( Widget::getRect().getY1() );
        
        // setup triangle vertices    
        glm::vec3 col[ NumQuadVert ];
        col[ 0 ] = glm::vec3( 0, 0, 0 );
        col[ 1 ] = glm::vec3( 0, 0, 0 );
        col[ 2 ] = glm::vec3( 0, 0, 0 );
        col[ 3 ] = glm::vec3( 0, 0, 0 );

        glm::vec3 pos[ NumQuadVert ];
        pos[ 0 ] = glm::vec3( x, y, 0 );
        pos[ 1 ] = glm::vec3( x, y + fontSize, 0 );
        pos[ 2 ] = glm::vec3( x + fontSize, y, 0 );
        pos[ 3 ] = glm::vec3( x + fontSize, y + fontSize, 0 );

        static const ui32 NumQuadIndices = 6;
        const ui32 NumTextVerts = getNumTextVerts( m_text );
        glm::vec3 *textPos = new glm::vec3[ NumTextVerts ];
        glm::vec3 *colors = new glm::vec3[ NumTextVerts ];
        glm::vec2 *tex0 = new glm::vec2[ NumTextVerts ];
        ui16 *textIndices = new ui16[ NumQuadIndices * m_text.size() ];

        const f32 invCol = 1.f / 16.f;
        const f32 invRow = 1.f / 16.f;
        ui32 textCol( 0 ), textRow( 0 );
        const f32 squaredFontSize( fontSize * fontSize );
        for ( ui32 i = 0; i < m_text.size(); i++ ) {
            const c8 ch = m_text[ i ];
            if ( Tokenizer::isLineBreak( ch ) ) {
                textCol = 0;
                textRow++;
                continue;
            }

            const ui16 VertexOffset( static_cast< ui16 >( i ) * static_cast< ui16 >( NumQuadVert ) );
            const f32  rowHeight( -1.0f * textRow * fontSize );
            textPos[ VertexOffset + 0 ].x = pos[ 0 ].x + ( squaredFontSize );
            textPos[ VertexOffset + 0 ].y = pos[ 0 ].y + rowHeight;
            textPos[ VertexOffset + 0 ].z = 0;

            textPos[ VertexOffset + 1 ].x = pos[ 1 ].x + ( squaredFontSize );
            textPos[ VertexOffset + 1 ].y = pos[ 1 ].y + rowHeight;
            textPos[ VertexOffset + 1 ].z = 0;

            textPos[ VertexOffset + 2 ].x = pos[ 2 ].x + ( squaredFontSize );
            textPos[ VertexOffset + 2 ].y = pos[ 2 ].y + rowHeight;
            textPos[ VertexOffset + 2 ].z = 0;

            textPos[ VertexOffset + 3 ].x = pos[ 3 ].x + ( squaredFontSize );
            textPos[ VertexOffset + 3 ].y = pos[ 3 ].y + rowHeight;
            textPos[ VertexOffset + 3 ].z = 0;

            //GeometryDiagnosticUtils::dumpTextBox( i, textPos, VertexOffset );

            const i32 column = ( ch ) % 16;
            const i32 row = ( ch ) / 16;
            const f32 s = column * invCol;
            const f32 t = ( row + 1 ) * invRow;

            tex0[ VertexOffset + 0 ].x = s;
            tex0[ VertexOffset + 0 ].y = 1.0f - t;

            tex0[ VertexOffset + 1 ].x = s;
            tex0[ VertexOffset + 1 ].y = 1.0f - t + 1.0f / 16.0f;

            tex0[ VertexOffset + 2 ].x = s + 1.0f / 16.0f;
            tex0[ VertexOffset + 2 ].y = 1.0f - t;

            tex0[ VertexOffset + 3 ].x = s + 1.0f / 16.0f;
            tex0[ VertexOffset + 3 ].y = 1.0f - t + 1.0f / 16.0f;

            //GeometryDiagnosticUtils::dumpTextTex0Box(i, tex0, VertexOffset);
            colors[ VertexOffset + 0 ] = col[ 0 ];
            colors[ VertexOffset + 1 ] = col[ 1 ];
            colors[ VertexOffset + 2 ] = col[ 2 ];
            colors[ VertexOffset + 3 ] = col[ 3 ];
            const ui32 IndexOffset( i * NumQuadIndices );
            textIndices[ 0 + IndexOffset ] = 0 + VertexOffset;
            textIndices[ 1 + IndexOffset ] = 2 + VertexOffset;
            textIndices[ 2 + IndexOffset ] = 1 + VertexOffset;

            textIndices[ 3 + IndexOffset ] = 1 + VertexOffset;
            textIndices[ 4 + IndexOffset ] = 2 + VertexOffset;
            textIndices[ 5 + IndexOffset ] = 3 + VertexOffset;
            textCol++;
        }
    }
}

} // Namespace UI
} // Namespace OSRE
