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
#include "VlkRenderBackend.h"
#include "VlkFunctions.h"

#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractDynamicLoader.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Platform;

static const String Tag     = "VlkRenderBackend";
static const String LibName = "vulkan-1.dll";

static AbstractDynamicLoader *getDynLoader() {
    AbstractDynamicLoader *dynLoader( PlatformInterface::getInstance()->getDynamicLoader() );
    if ( nullptr == dynLoader ) {
        osre_error( Tag, "No instance to dynloader." );
        return nullptr;
    }

    return dynLoader;
}

VlkRenderBackend::VlkRenderBackend()
: m_vulkan()
, m_handle( nullptr )
, m_state( State::Uninitialized ) {
    // empty
}

VlkRenderBackend::~VlkRenderBackend() {
    if ( m_state == State::Initialized ) {
        AbstractDynamicLoader *dynLoader( getDynLoader() );
        dynLoader->unload( LibName.c_str() );
        m_handle = nullptr;
        m_state = State::Uninitialized;
    }
}

bool VlkRenderBackend::create() {
    if ( m_state == State::Initialized ) {
        return true;
    }

    if ( !loadVulkanLib() ) {
        osre_error( Tag, "Error while loading vulkan lib." );
        return false;
    }

    if ( !loadExportedEntryPoints() ) {
        osre_error( Tag, "Error while loading vulkan function enty points." );
        return false;
    }

    if ( !loadGlobalLevelEntryPoints() ) {
        osre_error( Tag, "Error while loading global level entry points." );
        return false;
    }

    if ( !createInstance() ) {
        return false;
    }
    
    if ( !loadInstanceLevelEntryPoints() ) {
        return false;
    }

    if ( !createPresentationSurface() ) {
        return false;
    }
    if ( !createDevice() ) {
        return false;
    }
    if ( !loadDeviceLevelEntryPoints() ) {
        return false;
    }
    if ( !getDeviceQueue() ) {
        return false;
    }
    if ( !createSwapChain() ) {
        return false;
    }

    m_state = State::Initialized;

    return true;
}

VkDevice VlkRenderBackend::getDevice() const {
    return m_vulkan.m_device;
}

const VlkSwapChainParameters VlkRenderBackend::getSwapChain() const {
    return m_vulkan.m_swapChain;
}

bool VlkRenderBackend::loadVulkanLib() {
    AbstractDynamicLoader *dynLoader( getDynLoader() );
    if ( nullptr == dynLoader ) {
        return false;
    }

    m_handle = dynLoader->load( LibName.c_str() );
    if ( nullptr == m_handle ) {
        osre_debug( Tag, "Cannot load vulkan lib." );
        return false;
    }

    return true;
}

bool VlkRenderBackend::loadExportedEntryPoints() {
    // get dynamic lib lader instance from platform interface
    AbstractDynamicLoader *dynLoader( getDynLoader() );
    if ( nullptr == dynLoader ) {
        return false;
    }

    // select vulkan lib for lookup
    if ( nullptr == dynLoader->lookupLib( LibName.c_str() ) ) {
        return false;
    }

    // load all entry points from lib
#define VK_EXPORTED_FUNCTION( fun )                                             \
    if( !(fun = (PFN_##fun)dynLoader->loadFunction( #fun )) ) {                 \
        osre_error( Tag, "Could not load exported function: " + #fun + "!" );   \
        return false;                                                           \
    }

#include "VlkExportedFunctions.h"

    return true;
}

bool VlkRenderBackend::loadGlobalLevelEntryPoints() {
#define VK_GLOBAL_LEVEL_FUNCTION( fun )                                            \
    if( !(fun = (PFN_##fun)vkGetInstanceProcAddr( nullptr, #fun )) ) {             \
        osre_error( Tag, "Could not load global level function: " + #fun + "!" );  \
        return false;                                                              \
    }

#include "VlkExportedFunctions.h"

    return true;
}

static bool checkExtensionAvailability( const char *extName, const CPPCore::TArray<VkExtensionProperties> &availableExt ) {
    for ( size_t i = 0; i < availableExt.size(); ++i ) {
        if ( strcmp( availableExt[ i ].extensionName, extName ) == 0 ) {
            return true;
        }
    }
    return false;
}

bool VlkRenderBackend::createInstance() {
    ui32 extCount( 0 );
    if ( ( vkEnumerateInstanceExtensionProperties( nullptr, &extCount, nullptr ) != VK_SUCCESS ) || ( 0 == extCount ) ) {
        osre_error( Tag, "Error occurred during instance extensions enumeration!" );
        return false;
    }
    
    CPPCore::TArray<VkExtensionProperties> availableExt( extCount );
    if ( vkEnumerateInstanceExtensionProperties( nullptr, &extCount, &availableExt[ 0 ] ) != VK_SUCCESS ) {
        osre_error( Tag, "Error occurred during instance extensions enumeration!" );
        return false;
    }

    CPPCore::TArray<const char*> extensions;
    extensions.add( VK_KHR_SURFACE_EXTENSION_NAME );
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    extensions.add( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    extensions.add( VK_KHR_XCB_SURFACE_EXTENSION_NAME );
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    extensions.add( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );
#endif

    for ( size_t i = 0; i < extensions.size(); ++i ) {
        if ( !checkExtensionAvailability( extensions[ i ], availableExt ) ) {
            osre_error( Tag, "Could not find instance extension named \"" + String( extensions[ i ] ) + "\"!" );
            return false;
        }
    }

    VkApplicationInfo application_info = {
        VK_STRUCTURE_TYPE_APPLICATION_INFO,             // VkStructureType            sType
        nullptr,                                        // const void                *pNext
        "API without Secrets: Introduction to Vulkan",  // const char                *pApplicationName
        VK_MAKE_VERSION( 1, 0, 0 ),                     // uint32_t                   applicationVersion
        "Vulkan Tutorial by Intel",                     // const char                *pEngineName
        VK_MAKE_VERSION( 1, 0, 0 ),                     // uint32_t                   engineVersion
        VK_API_VERSION                                  // uint32_t                   apiVersion
    };

    VkInstanceCreateInfo instance_create_info = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,         // VkStructureType            sType
        nullptr,                                        // const void                *pNext
        0,                                              // VkInstanceCreateFlags      flags
        &application_info,                              // const VkApplicationInfo   *pApplicationInfo
        0,                                              // uint32_t                   enabledLayerCount
        nullptr,                                        // const char * const        *ppEnabledLayerNames
        static_cast< uint32_t >( extensions.size() ),   // uint32_t                   enabledExtensionCount
        &extensions[ 0 ]                                // const char * const        *ppEnabledExtensionNames
    };

    if ( vkCreateInstance( &instance_create_info, nullptr, &m_vulkan.m_instance ) != VK_SUCCESS ) {
        osre_error( Tag, "Could not create Vulkan instance!" );
        return false;
    }

    return true;

}

bool VlkRenderBackend::loadInstanceLevelEntryPoints() {
    // get dynamic lib lader instance from platform interface
    AbstractDynamicLoader *dynLoader( getDynLoader() );
    if ( nullptr == dynLoader ) {
        return false;
    }

    // select vulkan lib for lookup
    if ( nullptr == dynLoader->lookupLib( LibName.c_str() ) ) {
        return false;
    }

#define VK_INSTANCE_LEVEL_FUNCTION( fun )                                             \
    if( !(fun = (PFN_##fun)dynLoader->loadFunction( m_vulkan.m_instance, #fun )) ) {  \
        osre_error( Tag, "Could not load instance level function: " + #fun + "!" );   \
        return false;                                                                 \
    }

#include "VlkExportedFunctions.h"

    return false;
}

//#define VK_USE_PLATFORM_WIN32_KHR 1 
bool VlkRenderBackend::createPresentationSurface() {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VkWin32SurfaceCreateInfoKHR surface_create_info = {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                  sType
        nullptr,                                          // const void                      *pNext
        0,                                                // VkWin32SurfaceCreateFlagsKHR     flags
        Window.Instance,                                  // HINSTANCE                        hinstance
        Window.Handle                                     // HWND                             hwnd
    };

    if ( vkCreateWin32SurfaceKHR( m_vulkan.m_instance, &surface_create_info, nullptr, &Vulkan.PresentationSurface ) == VK_SUCCESS ) {
        return true;
    }

#elif defined(VK_USE_PLATFORM_XCB_KHR)
    VkXcbSurfaceCreateInfoKHR surface_create_info = {
        VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,    // VkStructureType                  sType
        nullptr,                                          // const void                      *pNext
        0,                                                // VkXcbSurfaceCreateFlagsKHR       flags
        Window.Connection,                                // xcb_connection_t*                connection
        Window.Handle                                     // xcb_window_t                     window
    };

    if ( vkCreateXcbSurfaceKHR( m_vulkan.m_instance, &surface_create_info, nullptr, &Vulkan.PresentationSurface ) == VK_SUCCESS ) {
        return true;
    }

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VkXlibSurfaceCreateInfoKHR surface_create_info = {
        VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,   // VkStructureType                sType
        nullptr,                                          // const void                    *pNext
        0,                                                // VkXlibSurfaceCreateFlagsKHR    flags
        Window.DisplayPtr,                                // Display                       *dpy
        Window.Handle                                     // Window                         window
    };
    if ( vkCreateXlibSurfaceKHR( m_vulkan.m_instance, &surface_create_info, nullptr, &Vulkan.PresentationSurface ) == VK_SUCCESS ) {
        return true;
    }

#endif
    osre_error( Tag, "Could not create presentation surface!" );

    return false;
}

bool VlkRenderBackend::createDevice() {
    return false;
}

bool VlkRenderBackend::loadDeviceLevelEntryPoints() {
    return false;
}

bool VlkRenderBackend::getDeviceQueue() {
    return false;
}

bool VlkRenderBackend::createSwapChain() {
    return false;
}

bool VlkRenderBackend::createRenderPass() {
    VkAttachmentDescription attachment_descriptions[] = {
        {
            0,                                          // VkAttachmentDescriptionFlags   flags
            getSwapChain().m_format,                    // VkFormat                       format
            VK_SAMPLE_COUNT_1_BIT,                      // VkSampleCountFlagBits          samples
            VK_ATTACHMENT_LOAD_OP_CLEAR,                // VkAttachmentLoadOp             loadOp
            VK_ATTACHMENT_STORE_OP_STORE,               // VkAttachmentStoreOp            storeOp
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,            // VkAttachmentLoadOp             stencilLoadOp
            VK_ATTACHMENT_STORE_OP_DONT_CARE,           // VkAttachmentStoreOp            stencilStoreOp
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                  initialLayout;
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR             // VkImageLayout                  finalLayout
        }
    };

    VkAttachmentReference color_attachment_references[] = {
        {
            0,                                          // uint32_t                       attachment
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL    // VkImageLayout                  layout
        }
    };

    VkSubpassDescription subpass_descriptions[] = {
        {
            0,                                          // VkSubpassDescriptionFlags      flags
            VK_PIPELINE_BIND_POINT_GRAPHICS,            // VkPipelineBindPoint            pipelineBindPoint
            0,                                          // uint32_t                       inputAttachmentCount
            nullptr,                                    // const VkAttachmentReference   *pInputAttachments
            1,                                          // uint32_t                       colorAttachmentCount
            color_attachment_references,                // const VkAttachmentReference   *pColorAttachments
            nullptr,                                    // const VkAttachmentReference   *pResolveAttachments
            nullptr,                                    // const VkAttachmentReference   *pDepthStencilAttachment
            0,                                          // uint32_t                       preserveAttachmentCount
            nullptr                                     // const uint32_t*                pPreserveAttachments
        }
    };

    VkRenderPassCreateInfo render_pass_create_info = {
        VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,    // VkStructureType                sType
        nullptr,                                      // const void                    *pNext
        0,                                            // VkRenderPassCreateFlags        flags
        1,                                            // uint32_t                       attachmentCount
        attachment_descriptions,                      // const VkAttachmentDescription *pAttachments
        1,                                            // uint32_t                       subpassCount
        subpass_descriptions,                         // const VkSubpassDescription    *pSubpasses
        0,                                            // uint32_t                       dependencyCount
        nullptr                                       // const VkSubpassDependency     *pDependencies
    };

    if ( vkCreateRenderPass( getDevice(), &render_pass_create_info, nullptr, &/*m_vulkan.*/m_renderPass ) != VK_SUCCESS ) {
        //std::cout << "Could not create render pass!" << std::endl;
        return false;
    }

    return true;
}

bool VlkRenderBackend::createFramebuffers() {
    const CPPCore::TArray<VlkImageParameters> &swap_chain_images = getSwapChain().m_images;
    m_framebuffers.resize( swap_chain_images.size() );

    for ( size_t i = 0; i < swap_chain_images.size(); ++i ) {
        VkFramebufferCreateInfo framebuffer_create_info = {
            VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  // VkStructureType                sType
            nullptr,                                    // const void                    *pNext
            0,                                          // VkFramebufferCreateFlags       flags
            m_renderPass,                          // VkRenderPass                   renderPass
            1,                                          // uint32_t                       attachmentCount
            &swap_chain_images[ i ].m_imageView,            // const VkImageView             *pAttachments
            300,                                        // uint32_t                       width
            300,                                        // uint32_t                       height
            1                                           // uint32_t                       layers
        };

        if ( vkCreateFramebuffer( getDevice(), &framebuffer_create_info, nullptr, &m_framebuffers[ i ] ) != VK_SUCCESS ) {
            //std::cout << "Could not create a framebuffer!" << std::endl;
            return false;
        }
    }
    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
