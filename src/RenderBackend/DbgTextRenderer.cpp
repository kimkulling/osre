#include <osre/RenderBackend/DbgTextRenderer.h>

namespace OSRE {
namespace RenderBackend {

DbgTextRenderer::DbgTextRenderer()
: m_text() {
 
}

DbgTextRenderer::~DbgTextRenderer() {

}
void DbgTextRenderer::setupTexture() {

}

void DbgTextRenderer::setCursor( ui32  x, ui32 y ) {

}

void DbgTextRenderer::setScale( ui32 sx, ui32 sy ) {

}

void DbgTextRenderer::addText( const String &text ) {
    m_text += text;
}

void DbgTextRenderer::drawText() {

}

}
}
