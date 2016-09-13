/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>

#include "vulkan.h"
#include "VlkCommon.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {

// Forward declarations
namespace Platform {
    struct LibHandle;

    class AbstractSurface;
}

namespace IO {
    class Stream;
}

namespace RenderBackend {

struct VlkShaderModule;

class VlkRenderBackend {
    enum class State {
        Uninitialized = 0,
        Initialized
    };

public:
    VlkRenderBackend();
    ~VlkRenderBackend();
    bool create( Platform::AbstractSurface *rootSurface );
    VkDevice getDevice() const;
    bool createRenderPass();
    bool createFramebuffers( ui32 width, ui32 height );
    bool createPipeline();
    bool createSemaphores();
    bool createCommandBuffers();
    bool recordCommandBuffers();
    VlkShaderModule *createShaderModule( IO::Stream &file );
    VlkPipelineLayout *createPipelineLayout();

private:
    bool loadVulkanLib();
    bool loadExportedEntryPoints();
    bool createInstance();
    bool loadInstanceLevelEntryPoints();
    bool createPresentationSurface();
    bool createDevice();
    bool loadDeviceLevelEntryPoints();
    bool getDeviceQueue();
    const VlkQueueParameters &getGraphicsQueue() const;
    bool createSwapChain();
    bool createSwapChainImageViews();
    bool loadGlobalLevelEntryPoints();
    bool checkPhysicalDeviceProperties( VkPhysicalDevice physical_device, uint32_t &selected_graphics_queue_family_index, uint32_t &selected_present_queue_family_index );
    bool createCommandPool( uint32_t queue_family_index, VkCommandPool *pool );
    const VlkSwapChainParameters &getSwapChain() const;
    bool allocateCommandBuffers( VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers );

private:
    VlkCommonParameters                 m_vulkan;
    VlkWindowParameters                 m_window;
    VkRenderPass                        m_renderPass;
    CPPCore::TArray<VkFramebuffer>      m_framebuffers;
    VkPipeline                          m_graphicsPipeline;
    VkSemaphore                         m_imageAvailableSemaphore;
    VkSemaphore                         m_renderingFinishedSemaphore;
    VkCommandPool                       m_graphicsCommandPool;
    VlkPipelineLayout                  *m_pipelineLayout;
    CPPCore::TArray<VkCommandBuffer>    m_graphicsCommandBuffers;
    CPPCore::TArray<VlkShaderModule*>   m_shaderModules;
    CPPCore::TArray<VlkPipelineLayout*> m_pipelineLayouts;
    Platform::LibHandle                *m_handle;
    State                               m_state;
};

} // Namespace RenderBackend
} // Namespace OSRE
