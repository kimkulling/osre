#include "VulkanRenderEventHandler.h"

namespace OSRE::RenderBackend {

using namespace ::OSRE::Common;

VulkanRenderEventHandler::VulkanRenderEventHandler() {

}

bool VulkanRenderEventHandler::onEvent(const Event &ev, const EventData *pEventData) {
    return true;
}

bool VulkanRenderEventHandler::onAttached(const EventData *eventData ) {
    return true;
}

bool VulkanRenderEventHandler::onDetached(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onCreateRenderer(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onDestroyRenderer(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onAttachView(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onDetachView(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onClearGeo(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onRenderFrame(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onInitRenderPasses(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onCommitNexFrame(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onShutdownRequest(const EventData *eventData) {
    return true;
}

bool VulkanRenderEventHandler::onResizeRenderTarget(const EventData *eventData) {
    return true;
}

}
