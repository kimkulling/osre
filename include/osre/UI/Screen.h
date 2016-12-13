#pragma once

#include <osre/UI/Widget.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
}

namespace Platform {
    class AbstractSurface;
}

namespace UI {

class Screen : public Widget {
public:
    Screen( const String &name, Widget *parent );
    virtual ~Screen();
    virtual void setSurface( Platform::AbstractSurface *surface );
    virtual void render( RenderBackend::RenderBackendService *rbSrv );

private:
    Platform::AbstractSurface *m_surface;
};

}
}
