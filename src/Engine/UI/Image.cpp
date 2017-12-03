#include <osre/UI/Image.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;

Image::Image( const String &name, Widget *parent )
: Widget( name, parent ) 
, m_imageUri() {

}

Image::~Image() {
    // empty
}

void Image::setUri( const IO::Uri &imageUri ) {
    if ( m_imageUri == imageUri ) {
        return;
    }
    
    m_imageUri = imageUri;
    Widget::requestRedraw();
}

const IO::Uri &Image::getUri() const {
    return m_imageUri;
}

void Image::onRender( UiRenderCmdCache &renderCmdCache, RenderBackendService *rbSrv ) {
    UiVertexCache vertexCache;
    UiIndexCache indexCache;
}

}
}
