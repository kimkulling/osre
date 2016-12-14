#pragma once

#include <osre/UI/Widget.h>

namespace OSRE {
namespace UI {

class Panel : public Widget {
public:
    Panel( const String &name, Widget *parent );
    virtual ~Panel();
    virtual void render( RenderBackend::RenderBackendService *rbSrv );
};

}
}
