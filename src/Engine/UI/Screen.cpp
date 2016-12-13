#include <osre/UI/Screen.h>

namespace OSRE {
namespace UI {

Screen::Screen( const String &name, Widget *parent )
: Widget( name, parent )
, m_surface( nullptr ) {

}

Screen::~Screen() {
}

void Screen::setSurface( Platform::AbstractSurface *surface ) {
    m_surface = surface;
}

void Screen::render( RenderBackend::RenderBackendService *rbSrv ) {
    if ( nullptr == m_surface ) {
        return;
    }
}

}
}

