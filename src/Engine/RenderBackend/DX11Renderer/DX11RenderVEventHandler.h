#pragma once

#include <osre/Common/AbstractEventHandler.h>

#include <cppcore/Container/TArray.h>

struct ID3D11Buffer;

namespace OSRE {
    
namespace Platform {
    class AbstractRenderContext;
}

namespace RenderBackend {

class DX11Renderer;

struct RenderCmd;

class DX11RenderEventHandler : public Common::AbstractEventHandler {
public:
    /// The class constructor.
    DX11RenderEventHandler();
    /// The class destructor, virtual.
    virtual ~DX11RenderEventHandler();
    /// The OnEvent-callback.
    bool onEvent(const Common::Event &ev, const Common::EventData *pEventData) override;

protected:
    bool onAttached(const Common::EventData *eventData) override;
    bool onDetached(const Common::EventData *eventData) override;
    bool onCreateRenderer(const Common::EventData *eventData);
    bool onDestroyRenderer(const Common::EventData *eventData);
    bool onAttachView(const Common::EventData *eventData);
    bool onDetachView(const Common::EventData *eventData);
    bool onClearGeo(const Common::EventData *eventData);
    bool onRenderFrame(const Common::EventData *eventData);
    bool onCommitNexFrame(const Common::EventData *eventData);
    bool onShutdownRequest(const Common::EventData *eventData);
    bool onResizeRenderTarget(const Common::EventData *eventData);

private:
    bool m_isRunning;
    DX11Renderer *m_dx11Renderer;
    CPPCore::TArray<RenderCmd*> m_renderCmds;
};

}
}
