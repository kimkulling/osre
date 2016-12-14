#include <osre/UI/Panel.h>

namespace OSRE {
namespace UI {

Panel::Panel( const String &name, Widget *parent )
: Widget( name, parent ) {
}

Panel::~Panel() {

}

void Panel::render( RenderBackend::RenderBackendService *rbSrv ) {
}


}
}
