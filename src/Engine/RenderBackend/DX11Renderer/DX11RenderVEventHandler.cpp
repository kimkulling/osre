#include "DX11RenderVEventHandler.h"

namespace OSRE {
namespace RenderBackend {

DX11RenderEventHandler::DX11RenderEventHandler()
: AbstractEventHandler() {

}

DX11RenderEventHandler::~DX11RenderEventHandler() {

}
/// The OnEvent-callback.
bool DX11RenderEventHandler::onEvent(const Common::Event &ev, const Common::EventData *pEventData) {
    return true;
}

}
}
