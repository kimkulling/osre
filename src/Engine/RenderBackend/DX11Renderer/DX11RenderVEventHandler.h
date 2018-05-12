#pragma once

#include <osre/Common/AbstractEventHandler.h>

namespace OSRE {
namespace RenderBackend {

class DX11RenderEventHandler : public Common::AbstractEventHandler {
public:
    /// The class constructor.
    DX11RenderEventHandler();
    /// The class destructor, virtual.
    virtual ~DX11RenderEventHandler();
    /// The OnEvent-callback.
    bool onEvent(const Common::Event &ev, const Common::EventData *pEventData) override;
};

}
}
