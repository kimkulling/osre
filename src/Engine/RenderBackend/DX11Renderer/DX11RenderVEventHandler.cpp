#include "DX11RenderVEventHandler.h"
#include "DX11Renderer.h"

#include <osre/Common/Logger.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractSurface.h>
#include <osre/Platform/AbstractRenderContext.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Geometry.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

static const String Tag = "DX11RenderEventHandler";

DX11RenderEventHandler::DX11RenderEventHandler()
: AbstractEventHandler()
, m_isRunning( true )
, m_dx11Renderer( nullptr )
, m_renderCtx( nullptr ) {

}

DX11RenderEventHandler::~DX11RenderEventHandler() {

}

bool DX11RenderEventHandler::onEvent(const Event &ev, const EventData *data) {
    bool result(false);
    if (!m_isRunning) {
        return true;
    }

    if (OnAttachEventHandlerEvent == ev) {
        result = onAttached(data);
    } else if (OnDetatachEventHandlerEvent == ev) {
        result = onDetached(data);
    } else if (OnCreateRendererEvent == ev) {
        result = onCreateRenderer(data);
    } else if (OnDestroyRendererEvent == ev) {
        result = onDestroyRenderer(data);
    } else if (OnAttachViewEvent == ev) {
        result = onAttachView(data);
    } else if (OnDetachViewEvent == ev) {
        result = onDetachView(data);
    } else if (OnRenderFrameEvent == ev) {
        result = onRenderFrame(data);
    } else if (OnCommitFrameEvent == ev) {
        result = onCommitNexFrame(data);
    } else if (OnClearSceneEvent == ev) {
        result = onClearGeo(data);
    } else if (OnShutdownRequest == ev) {
        result = onShutdownRequest(data);
    } else if (OnResizeEvent == ev) {
        result = onResizeRenderTarget(data);
    }

    return result;
}

bool DX11RenderEventHandler::onAttached(const EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onDetached(const EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onCreateRenderer(const Common::EventData *eventData) {
    if (nullptr != m_dx11Renderer) {
        return false;
    }
    m_dx11Renderer = new DX11Renderer();

    CreateRendererEventData *createRendererEvData = (CreateRendererEventData*)eventData;
    AbstractSurface *activeSurface = createRendererEvData->m_activeSurface;
    if (nullptr == activeSurface) {
        osre_debug(Tag, "No active surface, pointer is nullptr.");
        return false;
    }

    if (!activeSurface->create()) {
        osre_debug(Tag, "Cannot create active surface.");
        return false;
    }

    bool result(false);

    result = m_dx11Renderer->create(activeSurface);

    return result;
}

bool DX11RenderEventHandler::onDestroyRenderer(const Common::EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onAttachView(const Common::EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onDetachView(const Common::EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onClearGeo(const Common::EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onRenderFrame(const Common::EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onCommitNexFrame(const Common::EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onShutdownRequest(const Common::EventData *eventData) {
    return true;
}

bool DX11RenderEventHandler::onResizeRenderTarget(const Common::EventData *eventData) {
    return true;
}

}
}
