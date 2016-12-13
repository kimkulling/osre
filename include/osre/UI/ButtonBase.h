#pragma once

#include <osre/UI/Widget.h>

namespace OSRE {

namespace Platform {
    class AbstractSurface;
}

namespace RenderBackend {
    class RenderBackendService;
}

namespace UI {

class Panel : public Widget {
public:
    Panel( const String &name, Widget *parent );
    virtual ~Panel();
    virtual void render( RenderBackend::RenderBackendService *rbSrv );
};

class ButtonBase : public Widget {
public:
    ButtonBase( const String &name, Widget *parent );
    virtual ~ButtonBase();
    virtual void render( RenderBackend::RenderBackendService *rbSrv );

private:
};

}
}

