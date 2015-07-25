#include <osre/RenderBackend/DbgTextRenderer.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

extern const unsigned char *glyph[];

DbgTextRenderer::DbgTextRenderer()
: m_data( nullptr )
, m_text() {
    setupVertexData();
}

DbgTextRenderer::~DbgTextRenderer() {
    delete m_data;
    m_data = nullptr;
}

void DbgTextRenderer::setupVertexData() {
/*    const ui32 NumChars = 128;
    static const f32 inv16 = 1.0f / 16.0f;
    for( ui32 i = 0; i < NumChars; ++i ) {
        while( glyph[ c ][ i ] != 0xff ) {
            const f32 x = ( glyph[ c ][ i ] / 16 ) * inv16;
            const f32 y = ( glyph[ c ][ i ] % 16 ) * inv16;
        }
    }*/
}

void DbgTextRenderer::setCursor( ui32  x, ui32 y ) {

}

void DbgTextRenderer::setScale( ui32 sx, ui32 sy ) {

}

void DbgTextRenderer::addText( const String &text ) {
    m_text += text;
}

void DbgTextRenderer::drawText() {
    if( m_text.empty() ) {
        return;
    }
}

}
}
