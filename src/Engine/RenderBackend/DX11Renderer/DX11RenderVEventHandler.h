/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
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
    ID3D11Buffer *m_matrixBuffer;
};

}
}
