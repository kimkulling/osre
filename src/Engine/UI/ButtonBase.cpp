#include <osre/UI/ButtonBase.h>
#include <osre/Platform/AbstractSurface.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;

Panel::Panel( const String &name, Widget *parent )
: Widget( name, parent ) {
}

Panel::~Panel() {

}

void Panel::render( RenderBackend::RenderBackendService *rbSrv ) {
}

ButtonBase::ButtonBase( const String &name, Widget *parent )
: Widget( name, parent ) {
    static_cast<void>( StyleProvider::getCurrentStyle() );
}

ButtonBase::~ButtonBase() {
    // empty
}

void ButtonBase::render( RenderBackend::RenderBackendService *rbSrv ) {
    // empty
}

}
}
