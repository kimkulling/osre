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
#ifdef OSRE_WINDOWS
#  include <windows.h>
#endif // OSRE_WINDOWS
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

struct VlkQueueParameters {
    VkQueue m_handle;
    ui32    m_familyIndex;

    VlkQueueParameters()
    : m_handle( VK_NULL_HANDLE )
    , m_familyIndex( 0 ) {
        // empty
    }
};

struct VlkImageParameters {
    VkImage     m_handle;
    VkImageView m_imageView;

    VlkImageParameters()
    : m_handle( VK_NULL_HANDLE )
    , m_imageView( VK_NULL_HANDLE ) {
        // empty
    }
};

struct VlkSwapChainParameters {
    VkSwapchainKHR                      m_handle;
    VkFormat                            m_format;
    CPPCore::TArray<VlkImageParameters> m_images;
    VkExtent2D                          m_extent;

    VlkSwapChainParameters()
    : m_handle( VK_NULL_HANDLE )
    , m_format( VK_FORMAT_UNDEFINED )
    , m_images()
    , m_extent() {
        // empty
    }
};

struct VlkCommonParameters {
    VkInstance                    m_instance;
    VkPhysicalDevice              m_physicalDevice;
    VkDevice                      m_device;
    VlkQueueParameters               m_graphicsQueue;
    VlkQueueParameters               m_presentQueue;
    VkSurfaceKHR                  m_presentationSurface;
    VlkSwapChainParameters           m_swapChain;

    VlkCommonParameters()
    : m_instance( VK_NULL_HANDLE )
    , m_physicalDevice( VK_NULL_HANDLE )
    , m_device( VK_NULL_HANDLE )
    , m_graphicsQueue()
    , m_presentQueue()
    , m_presentationSurface( VK_NULL_HANDLE )
    , m_swapChain() {
        // empty
    }
};

struct VlkWindowParameters {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    HINSTANCE           m_instance;
    HWND                m_handle;

    VlkWindowParameters() :
        m_instance(),
        m_handle() {
    }

#elif defined(VK_USE_PLATFORM_XCB_KHR)
    xcb_connection_t   *m_connection;
    xcb_window_t        m_handle;

    VlkWindowParameters() :
        m_connection(),
        m_handle() {
    }

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    Display            *m_displayPtr;
    Window              m_handle;

    VlkWindowParameters()
    : m_displayPtr()
    , m_handle() {
        // empty
    }

#endif
};

struct VlkShaderModule {
    VkShaderModule m_module;
    VlkShaderModule() 
    : m_module() {
        // empty
    }
};

struct VlkPipelineLayout {
    VkPipelineLayout m_pipelineLayout;
    VlkPipelineLayout()
    : m_pipelineLayout() {
        // empty
    }
};

#define VK_EXPORTED_FUNCTION( fun ) extern PFN_##fun fun;
#define VK_GLOBAL_LEVEL_FUNCTION( fun) extern PFN_##fun fun;
#define VK_INSTANCE_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;
#define VK_DEVICE_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;

}
}
