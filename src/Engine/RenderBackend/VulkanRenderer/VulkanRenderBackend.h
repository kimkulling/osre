#pragma once

#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Event.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace RenderBackend {

class VulkanRenderBackend {
public:
    VulkanRenderBackend();
    ~VulkanRenderBackend();
    bool create();
    bool destroy();
};

}
} // namespace OSRE
