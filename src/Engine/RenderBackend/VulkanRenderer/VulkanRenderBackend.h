#pragma once

#include "Common/AbstractEventHandler.h"
#include "Common/Event.h"
#include "RenderBackend/RenderBackendService.h"

namespace OSRE::RenderBackend {

class VulkanRenderBackend {
public:
    VulkanRenderBackend();
    ~VulkanRenderBackend();
    bool create();
    bool destroy();
};

} // namespace OSRE::RenderBackend
