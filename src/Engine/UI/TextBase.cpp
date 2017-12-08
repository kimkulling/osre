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
#include <osre/Scene/MaterialBuilder.h>
#include <osre/Common/Tokenizer.h>
#include <osre/RenderBackend/FontBase.h>
#include <osre/RenderBackend/RenderCommon.h>

#include "UIRenderUtils.h"

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

TextBase::TextBase( const String &name, Widget *parent )
: Widget( name, parent )
, m_font( nullptr )
, m_text( "" ) {
    if ( nullptr != parent ) {
        setStackIndex( parent->getStackIndex() + 1 );
    }
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

    const i32 stackId( getStackIndex() );
    const f32 z(-1.0f* static_cast<f32>( stackId ) );
    RenderBackend::RenderVert *verts;
    f32 x( Widget::getRect().getX1() );
    f32 y( Widget::getRect().getY1() );
    WidgetCoordMapping::mapPosToWorld( x, y, x, y );

    // setup triangle vertices    
    glm::vec3 col[ NumQuadVert ];
    col[ 0 ] = glm::vec3( 0, 0, 0 );
    col[ 1 ] = glm::vec3( 0, 0, 0 );
    col[ 2 ] = glm::vec3( 0, 0, 0 );
    col[ 3 ] = glm::vec3( 0, 0, 0 );

    glm::vec3 pos[ NumQuadVert ];
    pos[ 0 ] = glm::vec3( x, y, z );
    pos[ 1 ] = glm::vec3( x, y + fontSize, z );
    pos[ 2 ] = glm::vec3( x + fontSize, y, z );
    pos[ 3 ] = glm::vec3( x + fontSize, y + fontSize, z );

    static const ui32 NumQuadIndices = 6;
    const ui32 NumTextVerts = getNumTextVerts( m_text );
    RenderVert *vertices = new RenderVert[ NumTextVerts ];
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
        vertices[ VertexOffset + 0 ].position.x = pos[ 0 ].x + ( squaredFontSize );
        vertices[ VertexOffset + 0 ].position.y = pos[ 0 ].y + rowHeight;
        vertices[ VertexOffset + 0 ].position.z = 0;

        vertices[ VertexOffset + 1 ].position.x = pos[ 1 ].x + ( squaredFontSize );
        vertices[ VertexOffset + 1 ].position.y = pos[ 1 ].y + rowHeight;
        vertices[ VertexOffset + 1 ].position.z = 0;

        vertices[ VertexOffset + 2 ].position.x = pos[ 2 ].x + ( squaredFontSize );
        vertices[ VertexOffset + 2 ].position.y = pos[ 2 ].y + rowHeight;
        vertices[ VertexOffset + 2 ].position.z = 0;

        vertices[ VertexOffset + 3 ].position.x = pos[ 3 ].x + ( squaredFontSize );
        vertices[ VertexOffset + 3 ].position.y = pos[ 3 ].y + rowHeight;
        vertices[ VertexOffset + 3 ].position.z = 0;

        //GeometryDiagnosticUtils::dumpTextBox( i, textPos, VertexOffset );

        const i32 column = ( ch ) % 16;
        const i32 row = ( ch ) / 16;
        const f32 s = column * invCol;
        const f32 t = ( row + 1 ) * invRow;

        vertices[ VertexOffset + 0 ].tex0.x = s;
        vertices[ VertexOffset + 0 ].tex0.y = 1.0f - t;

        vertices[ VertexOffset + 1 ].tex0.x = s;
        vertices[ VertexOffset + 1 ].tex0.y = 1.0f - t + 1.0f / 16.0f;

        vertices[ VertexOffset + 2 ].tex0.x = s + 1.0f / 16.0f;
        vertices[ VertexOffset + 2 ].tex0.y = 1.0f - t;

        vertices[ VertexOffset + 3 ].tex0.x = s + 1.0f / 16.0f;
        vertices[ VertexOffset + 3 ].tex0.y = 1.0f - t + 1.0f / 16.0f;

        GeometryDiagnosticUtils::dumpTextTex0Box(i, tex0, VertexOffset);
        vertices[ VertexOffset + 0 ].color0 = col[ 0 ];
        vertices[ VertexOffset + 1 ].color0 = col[ 1 ];
        vertices[ VertexOffset + 2 ].color0 = col[ 2 ];
        vertices[ VertexOffset + 3 ].color0 = col[ 3 ];
        const ui32 IndexOffset( i * NumQuadIndices );
        textIndices[ 0 + IndexOffset ] = 0 + VertexOffset;
        textIndices[ 1 + IndexOffset ] = 2 + VertexOffset;
        textIndices[ 2 + IndexOffset ] = 1 + VertexOffset;

        textIndices[ 3 + IndexOffset ] = 1 + VertexOffset;
        textIndices[ 4 + IndexOffset ] = 2 + VertexOffset;
        textIndices[ 5 + IndexOffset ] = 3 + VertexOffset;
        textCol++;
    }

    UiVertexCache vertexCache;
    for ( ui32 i = 0; i < NumTextVerts; ++i ) {
        vertexCache.add( vertices[ i ] );
    }
    delete[] vertices;

    UiIndexCache indexCache;
    for ( ui32 i = 0; i < NumQuadIndices * m_text.size(); ++i ) {
        indexCache.add( textIndices[ i ] );
    }

    Material *mat( Scene::MaterialBuilder::createBuildinMaterial( VertexType::RenderVertex) );

    UiRenderCmd *cmd( new UiRenderCmd );
    cmd->m_vc = vertexCache;
    cmd->m_ic = indexCache;
    cmd->m_mat = mat;
    renderCmdCache.add( cmd );
}

} // Namespace UI
} // Namespace OSRE
