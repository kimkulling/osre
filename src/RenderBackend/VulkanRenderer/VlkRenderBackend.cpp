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
#ifdef OSRE_WINDOWS
#  include "src/Platform/win32/Win32Surface.h"
#else
#  include "src/Platform/sdl2/SDL2Surface.h"
#endif

namespace OSRE {
namespace RenderBackend {

using namespace ::CPPCore;
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
, m_window()
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

bool VlkRenderBackend::create( Platform::AbstractSurface *rootSurface ) {
    if ( m_state == State::Initialized ) {
        return true;
    }

    if ( nullptr == rootSurface ) {
        return false;
    }
#ifdef OSRE_WINDOWS
    Win32Surface *win32Surface = ( Win32Surface* ) rootSurface;
    m_window.m_instance = win32Surface->getModuleHandle();
    m_window.m_handle = win32Surface->getHWnd();
#endif
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
        osre_error( Tag, "Error while creating vulkan instance." );
        return false;
    }
    if ( !loadInstanceLevelEntryPoints() ) {
        osre_error( Tag, "Error while loading level entry points." );
        return false;
    }
    if ( !createPresentationSurface() ) {
        osre_error( Tag, "Error while creating presentation surface." );
        return false;
    }
    if ( !createDevice() ) {
        osre_error( Tag, "Error while creating vulkan device." );
        return false;
    }
    if ( !loadDeviceLevelEntryPoints() ) {
        osre_error( Tag, "Error while loading device level entry points." );
        return false;
    }
    if ( !getDeviceQueue() ) {
        osre_error( Tag, "Error while getting device queue." );
        return false;
    }
    if ( !createSwapChain() ) {
        osre_error( Tag, "Error while creating swap-chain." );
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
    // get dynamic lib loader instance from platform interface
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
    if( !(fun = (PFN_##fun) dynLoader->loadFunction( #fun )) ) {                 \
        osre_error( Tag, "Could not load exported function: " + String( #fun ) + "!" );   \
        return false;                                                           \
    }

#include "VlkExportedFunctions.inl"

    return true;
}

bool VlkRenderBackend::loadGlobalLevelEntryPoints() {
#define VK_GLOBAL_LEVEL_FUNCTION( fun )                                            \
    if( !(fun = (PFN_##fun)vkGetInstanceProcAddr( nullptr, #fun )) ) {             \
        osre_error( Tag, "Could not load global level function: " + String( #fun ) + "!" );  \
        return false;                                                              \
    }

#include "VlkExportedFunctions.inl"

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
    // get dynamic lib loader instance from platform interface
    AbstractDynamicLoader *dynLoader( getDynLoader() );
    if ( nullptr == dynLoader ) {
        return false;
    }

    // select vulkan lib for lookup
    if ( nullptr == dynLoader->lookupLib( LibName.c_str() ) ) {
        return false;
    }

#define VK_INSTANCE_LEVEL_FUNCTION( fun )                                             \
    if( !(fun = (PFN_##fun)dynLoader->loadFunction( #fun )) ) {  \
        osre_error( Tag, "Could not load instance level function: " + String( #fun ) + "!" );   \
        return false;                                                                 \
    }

#include "VlkExportedFunctions.inl"

    return true;
}

bool VlkRenderBackend::createPresentationSurface() {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VkWin32SurfaceCreateInfoKHR surface_create_info = {
        VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,  // VkStructureType                  sType
        nullptr,                                          // const void                      *pNext
        0,                                                // VkWin32SurfaceCreateFlagsKHR     flags
        m_window.m_instance,                                  // HINSTANCE                        hinstance
        m_window.m_handle                                     // HWND                             hwnd
    };

    if ( vkCreateWin32SurfaceKHR( m_vulkan.m_instance, &surface_create_info, nullptr, &m_vulkan.m_presentationSurface ) == VK_SUCCESS ) {
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
    ui32 numDevices( 0 );
    if ( ( vkEnumeratePhysicalDevices( m_vulkan.m_instance, &numDevices, nullptr ) != VK_SUCCESS ) || ( numDevices == 0 ) ) {
        osre_error( Tag, "Error occurred during physical devices enumeration!" );
        return false;
    }

    TArray<VkPhysicalDevice> physicalDevices( numDevices );
    if ( vkEnumeratePhysicalDevices( m_vulkan.m_instance, &numDevices, &physicalDevices[ 0 ] ) != VK_SUCCESS ) {
        osre_error( Tag, "Error occurred during physical devices enumeration!" );
        return false;
    }

    ui32 selected_graphics_queue_family_index = UINT32_MAX;
    ui32 selected_present_queue_family_index = UINT32_MAX;
    for ( ui32 i = 0; i < numDevices; ++i ) {
        if ( checkPhysicalDeviceProperties( physicalDevices[ i ], selected_graphics_queue_family_index, selected_present_queue_family_index ) ) {
            m_vulkan.m_physicalDevice = physicalDevices[ i ];
        }
    }
    if ( m_vulkan.m_physicalDevice == VK_NULL_HANDLE ) {
        osre_error( Tag, "Could not select physical device based on the chosen properties!" );
        return false;
    }

    TArray<VkDeviceQueueCreateInfo> queue_create_infos;
    TArray<f32> queue_priorities;
    queue_priorities.add( 1.0f );

    queue_create_infos.add( {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,       // VkStructureType              sType
        nullptr,                                          // const void                  *pNext
        0,                                                // VkDeviceQueueCreateFlags     flags
        selected_graphics_queue_family_index,             // uint32_t                     queueFamilyIndex
        static_cast< uint32_t >( queue_priorities.size() ),   // uint32_t                     queueCount
        &queue_priorities[ 0 ]                              // const float                 *pQueuePriorities
    } );

    if ( selected_graphics_queue_family_index != selected_present_queue_family_index ) {
        queue_create_infos.add( {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,     // VkStructureType              sType
            nullptr,                                        // const void                  *pNext
            0,                                              // VkDeviceQueueCreateFlags     flags
            selected_present_queue_family_index,            // uint32_t                     queueFamilyIndex
            static_cast< uint32_t >( queue_priorities.size() ), // uint32_t                     queueCount
            &queue_priorities[ 0 ]                            // const float                 *pQueuePriorities
        } );
    }

    TArray<const char*> extensions;
    extensions.add( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

    VkDeviceCreateInfo device_create_info = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,                 // VkStructureType                    sType
        nullptr,                                              // const void                        *pNext
        0,                                                    // VkDeviceCreateFlags                flags
        static_cast< uint32_t >( queue_create_infos.size() ), // uint32_t                           queueCreateInfoCount
        &queue_create_infos[ 0 ],                             // const VkDeviceQueueCreateInfo     *pQueueCreateInfos
        0,                                                    // uint32_t                           enabledLayerCount
        nullptr,                                              // const char * const                *ppEnabledLayerNames
        static_cast< uint32_t >( extensions.size() ),         // uint32_t                           enabledExtensionCount
        &extensions[ 0 ],                                     // const char * const                *ppEnabledExtensionNames
        nullptr                                               // const VkPhysicalDeviceFeatures    *pEnabledFeatures
    };

    if ( vkCreateDevice( m_vulkan.m_physicalDevice, &device_create_info, nullptr, &m_vulkan.m_device ) != VK_SUCCESS ) {
        osre_error( Tag, "Could not create Vulkan device!" );
        return false;
    }
    
    m_vulkan.m_graphicsQueue.m_familyIndex = selected_graphics_queue_family_index;
    m_vulkan.m_presentQueue.m_familyIndex = selected_present_queue_family_index;

    return true;
}

bool VlkRenderBackend::loadDeviceLevelEntryPoints() {
    // get dynamic lib loader instance from platform interface
    AbstractDynamicLoader *dynLoader( getDynLoader() );
    if ( nullptr == dynLoader ) {
        osre_error( Tag, "Pointer to dynamic loader is nullptr." );
        return false;
    }

    // select vulkan lib for lookup
    if ( nullptr == dynLoader->lookupLib( LibName.c_str() ) ) {
        osre_error( Tag, "Error while looking for " + LibName );
        return false;
    }

#define VK_DEVICE_LEVEL_FUNCTION( fun )                                                   \
    if( !(fun = (PFN_##fun)vkGetDeviceProcAddr( m_vulkan.m_device, #fun )) ) {            \
      osre_error( Tag, "Could not load device level function: " + String( #fun ) + "!" );  \
      return false;                                                                       \
    }
#include "VlkExportedFunctions.inl"

    return true;
}

bool VlkRenderBackend::getDeviceQueue() {
    vkGetDeviceQueue( m_vulkan.m_device, m_vulkan.m_graphicsQueue.m_familyIndex, 0, &m_vulkan.m_graphicsQueue.m_handle );
    vkGetDeviceQueue( m_vulkan.m_device, m_vulkan.m_presentQueue.m_familyIndex, 0, &m_vulkan.m_presentQueue.m_handle );
    return true;
}

static ui32 getSwapChainNumImages( VkSurfaceCapabilitiesKHR &surface_capabilities ) {
    // Set of images defined in a swap chain may not always be available for application to render to:
    // One may be displayed and one may wait in a queue to be presented
    // If application wants to use more images at the same time it must ask for more images
    ui32 image_count = surface_capabilities.minImageCount + 1;
    if ( ( surface_capabilities.maxImageCount > 0 ) &&
        ( image_count > surface_capabilities.maxImageCount ) ) {
        image_count = surface_capabilities.maxImageCount;
    }
    return image_count;
}

static VkSurfaceFormatKHR getSwapChainFormat( TArray<VkSurfaceFormatKHR> &surface_formats ) {
    // If the list contains only one entry with undefined format
    // it means that there are no preferred surface formats and any can be chosen
    if ( ( surface_formats.size() == 1 ) &&
        ( surface_formats[ 0 ].format == VK_FORMAT_UNDEFINED ) ) {
        return{ VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
    }

    // Check if list contains most widely used R8 G8 B8 A8 format
    // with nonlinear color space
    for ( VkSurfaceFormatKHR &surface_format : surface_formats ) {
        if ( surface_format.format == VK_FORMAT_R8G8B8A8_UNORM ) {
            return surface_format;
        }
    }

    // Return the first format from the list
    return surface_formats[ 0 ];
}

static VkExtent2D getSwapChainExtent( VkSurfaceCapabilitiesKHR &surface_capabilities ) {
    // Special value of surface extent is width == height == -1
    // If this is so we define the size by ourselves but it must fit within defined confines
    if ( surface_capabilities.currentExtent.width == -1 ) {
        VkExtent2D swap_chain_extent = { 640, 480 };
        if ( swap_chain_extent.width < surface_capabilities.minImageExtent.width ) {
            swap_chain_extent.width = surface_capabilities.minImageExtent.width;
        }
        if ( swap_chain_extent.height < surface_capabilities.minImageExtent.height ) {
            swap_chain_extent.height = surface_capabilities.minImageExtent.height;
        }
        if ( swap_chain_extent.width > surface_capabilities.maxImageExtent.width ) {
            swap_chain_extent.width = surface_capabilities.maxImageExtent.width;
        }
        if ( swap_chain_extent.height > surface_capabilities.maxImageExtent.height ) {
            swap_chain_extent.height = surface_capabilities.maxImageExtent.height;
        }
        return swap_chain_extent;
    }

    // Most of the cases we define size of the swap_chain images equal to current window's size
    return surface_capabilities.currentExtent;
}

static VkImageUsageFlags getSwapChainUsageFlags( VkSurfaceCapabilitiesKHR &surface_capabilities ) {
    // Color attachment flag must always be supported
    // We can define other usage flags but we always need to check if they are supported
    if ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ) {
        return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    /*std::cout << "VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT image usage is not supported by the swap chain!" << std::endl
        << "Supported swap chain's image usages include:" << std::endl
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT ? "    VK_IMAGE_USAGE_TRANSFER_SRC\n" : "" )
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ? "    VK_IMAGE_USAGE_TRANSFER_DST\n" : "" )
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT ? "    VK_IMAGE_USAGE_SAMPLED\n" : "" )
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT ? "    VK_IMAGE_USAGE_STORAGE\n" : "" )
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_COLOR_ATTACHMENT\n" : "" )
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT\n" : "" )
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT\n" : "" )
        << ( surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT ? "    VK_IMAGE_USAGE_INPUT_ATTACHMENT" : "" )
        << std::endl;*/
    return static_cast< VkImageUsageFlags >( -1 );
}

static VkSurfaceTransformFlagBitsKHR getSwapChainTransform( VkSurfaceCapabilitiesKHR &surface_capabilities ) {
    // Sometimes images must be transformed before they are presented (i.e. due to device's orienation
    // being other than default orientation)
    // If the specified transform is other than current transform, presentation engine will transform image
    // during presentation operation; this operation may hit performance on some platforms
    // Here we don't want any transformations to occur so if the identity transform is supported use it
    // otherwise just use the same transform as current transform
    if ( surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) {
        return VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        return surface_capabilities.currentTransform;
    }
}

static VkPresentModeKHR getSwapChainPresentMode( TArray<VkPresentModeKHR> &present_modes ) {
    // FIFO present mode is always available
    // MAILBOX is the lowest latency V-Sync enabled mode (something like triple-buffering) so use it if available
    for ( VkPresentModeKHR &present_mode : present_modes ) {
        if ( present_mode == VK_PRESENT_MODE_MAILBOX_KHR ) {
            return present_mode;
        }
    }
    for ( VkPresentModeKHR &present_mode : present_modes ) {
        if ( present_mode == VK_PRESENT_MODE_FIFO_KHR ) {
            return present_mode;
        }
    }
    osre_error( Tag, "FIFO present mode is not supported by the swap chain!" );
    
    return static_cast< VkPresentModeKHR >( -1 );
}

bool VlkRenderBackend::createSwapChain() {
    if ( m_vulkan.m_device != VK_NULL_HANDLE ) {
        vkDeviceWaitIdle( m_vulkan.m_device );
    }

    for ( size_t i = 0; i < m_vulkan.m_swapChain.m_images.size(); ++i ) {
        if ( m_vulkan.m_swapChain.m_images[ i ].m_imageView != VK_NULL_HANDLE ) {
            vkDestroyImageView( getDevice(), m_vulkan.m_swapChain.m_images[ i ].m_imageView, nullptr );
            m_vulkan.m_swapChain.m_images[ i ].m_imageView = VK_NULL_HANDLE;
        }
    }
    m_vulkan.m_swapChain.m_images.clear();

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    if ( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_vulkan.m_physicalDevice, m_vulkan.m_presentationSurface, &surfaceCapabilities ) != VK_SUCCESS ) {
        osre_error( Tag, "Could not check presentation surface capabilities!" );
        return false;
    }

    ui32 formatCount( 0 );
    if ( ( vkGetPhysicalDeviceSurfaceFormatsKHR( m_vulkan.m_physicalDevice, m_vulkan.m_presentationSurface, &formatCount, nullptr ) != VK_SUCCESS ) || ( formatCount == 0 ) ) {
        osre_error( Tag, "Error occurred during presentation surface formats enumeration!" );
        return false;
    }

    TArray<VkSurfaceFormatKHR> surface_formats( formatCount );
    if ( vkGetPhysicalDeviceSurfaceFormatsKHR( m_vulkan.m_physicalDevice, m_vulkan.m_presentationSurface, &formatCount, &surface_formats[ 0 ] ) != VK_SUCCESS ) {
        osre_error( Tag, "Error occurred during presentation surface formats enumeration!" );
        return false;
    }

    ui32 present_modes_count( 0 );
    if ( ( vkGetPhysicalDeviceSurfacePresentModesKHR( m_vulkan.m_physicalDevice, m_vulkan.m_presentationSurface, &present_modes_count, nullptr ) != VK_SUCCESS ) ||
        ( present_modes_count == 0 ) ) {
        osre_error( Tag, "Error occurred during presentation surface present modes enumeration!" );
        return false;
    }

    TArray<VkPresentModeKHR> present_modes( present_modes_count );
    if ( vkGetPhysicalDeviceSurfacePresentModesKHR( m_vulkan.m_physicalDevice, m_vulkan.m_presentationSurface, &present_modes_count, &present_modes[ 0 ] ) != VK_SUCCESS ) {
        osre_error( Tag, "Error occurred during presentation surface present modes enumeration!" );
        return false;
    }

    ui32                          desired_number_of_images = getSwapChainNumImages( surfaceCapabilities );
    VkSurfaceFormatKHR            desired_format = getSwapChainFormat( surface_formats );
    VkExtent2D                    desired_extent = getSwapChainExtent( surfaceCapabilities );
    VkImageUsageFlags             desired_usage = getSwapChainUsageFlags( surfaceCapabilities );
    VkSurfaceTransformFlagBitsKHR desired_transform = getSwapChainTransform( surfaceCapabilities );
    VkPresentModeKHR              desired_present_mode = getSwapChainPresentMode( present_modes );
    VkSwapchainKHR                old_swap_chain = m_vulkan.m_swapChain.m_handle;

    if ( static_cast< int >( desired_usage ) == -1 ) {
        return false;
    }
    if ( static_cast< int >( desired_present_mode ) == -1 ) {
        return false;
    }

    VkSwapchainCreateInfoKHR swap_chain_create_info = {
        VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,  // VkStructureType                sType
        nullptr,                                      // const void                    *pNext
        0,                                            // VkSwapchainCreateFlagsKHR      flags
        m_vulkan.m_presentationSurface,                   // VkSurfaceKHR                   surface
        desired_number_of_images,                     // uint32_t                       minImageCount
        desired_format.format,                        // VkFormat                       imageFormat
        desired_format.colorSpace,                    // VkColorSpaceKHR                imageColorSpace
        desired_extent,                               // VkExtent2D                     imageExtent
        1,                                            // uint32_t                       imageArrayLayers
        desired_usage,                                // VkImageUsageFlags              imageUsage
        VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode                  imageSharingMode
        0,                                            // uint32_t                       queueFamilyIndexCount
        nullptr,                                      // const uint32_t                *pQueueFamilyIndices
        desired_transform,                            // VkSurfaceTransformFlagBitsKHR  preTransform
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,            // VkCompositeAlphaFlagBitsKHR    compositeAlpha
        desired_present_mode,                         // VkPresentModeKHR               presentMode
        VK_TRUE,                                      // VkBool32                       clipped
        old_swap_chain                                // VkSwapchainKHR                 oldSwapchain
    };

    if ( vkCreateSwapchainKHR( m_vulkan.m_device, &swap_chain_create_info, nullptr, &m_vulkan.m_swapChain.m_handle ) != VK_SUCCESS ) {
        osre_error( Tag, "Could not create swap chain!" );
        return false;
    }
    if ( old_swap_chain != VK_NULL_HANDLE ) {
        vkDestroySwapchainKHR( m_vulkan.m_device, old_swap_chain, nullptr );
    }

    m_vulkan.m_swapChain.m_format = desired_format.format;

    ui32 image_count = 0;
    if ( ( vkGetSwapchainImagesKHR( m_vulkan.m_device, m_vulkan.m_swapChain.m_handle, &image_count, nullptr ) != VK_SUCCESS ) || ( image_count == 0 ) ) {
        osre_error( Tag, "Could not get swap chain images!" );
        return false;
    }
    m_vulkan.m_swapChain.m_images.resize( image_count );

    TArray<VkImage> images( image_count );
    if ( vkGetSwapchainImagesKHR( m_vulkan.m_device, m_vulkan.m_swapChain.m_handle, &image_count, &images[ 0 ] ) != VK_SUCCESS ) {
        osre_error( Tag, "Could not get swap chain images!" );
        return false;
    }

    for ( size_t i = 0; i < m_vulkan.m_swapChain.m_images.size(); ++i ) {
        m_vulkan.m_swapChain.m_images[ i ].m_handle = images[ i ];
    }
    m_vulkan.m_swapChain.m_extent = desired_extent;

    return createSwapChainImageViews();
}

bool  VlkRenderBackend::createSwapChainImageViews() {
    for ( size_t i = 0; i < m_vulkan.m_swapChain.m_images.size(); ++i ) {
        VkImageViewCreateInfo image_view_create_info = {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,    // VkStructureType                sType
            nullptr,                                     // const void                    *pNext
            0,                                           // VkImageViewCreateFlags         flags
            m_vulkan.m_swapChain.m_images[ i ].m_handle, // VkImage                        image
            VK_IMAGE_VIEW_TYPE_2D,                       // VkImageViewType                viewType
            getSwapChain().m_format,                       // VkFormat                       format
            {                                            // VkComponentMapping             components
                VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             r
                VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             g
                VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             b
                VK_COMPONENT_SWIZZLE_IDENTITY               // VkComponentSwizzle             a
            },
            {                                           // VkImageSubresourceRange        subresourceRange
                VK_IMAGE_ASPECT_COLOR_BIT,                  // VkImageAspectFlags             aspectMask
                0,                                          // uint32_t                       baseMipLevel
                1,                                          // uint32_t                       levelCount
                0,                                          // uint32_t                       baseArrayLayer
                1                                           // uint32_t                       layerCount
            }
        };

        if ( vkCreateImageView( getDevice(), &image_view_create_info, nullptr, &m_vulkan.m_swapChain.m_images[ i ].m_imageView ) != VK_SUCCESS ) {
            osre_error( Tag, "Could not create image view for framebuffer!" );
            return false;
        }
    }

    return true;
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
            VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                  initialLayout;
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
        osre_error( Tag, "Could not create render pass!" );
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
            osre_error( Tag, "Could not create a framebuffer!" );
            return false;
        }
    }
    return true;
}

bool VlkRenderBackend::checkPhysicalDeviceProperties( VkPhysicalDevice physical_device, uint32_t &selected_graphics_queue_family_index, uint32_t &selected_present_queue_family_index ) {
    ui32 extensionsCount( 0 );
    if ( ( vkEnumerateDeviceExtensionProperties( physical_device, nullptr, &extensionsCount, nullptr ) != VK_SUCCESS ) || ( extensionsCount == 0 ) ) {
        std::stringstream stream;
        stream << physical_device;
        osre_error( Tag, String( "Error occurred during physical device " ) + stream.str() + String(" extensions enumeration!" ) );
        return false;
    }

    TArray<VkExtensionProperties> available_extensions( extensionsCount );
    if ( vkEnumerateDeviceExtensionProperties( physical_device, nullptr, &extensionsCount, &available_extensions[ 0 ] ) != VK_SUCCESS ) {
        std::stringstream stream;
        stream << physical_device;
        osre_error( Tag, "Error occurred during physical device " + stream.str() + " extensions enumeration!" );
        return false;
    }

    TArray<const char*> device_extensions;
    device_extensions.add( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

    for ( size_t i = 0; i < device_extensions.size(); ++i ) {
        if ( !checkExtensionAvailability( device_extensions[ i ], available_extensions ) ) {
            std::stringstream stream;
            stream << physical_device;
            osre_error( Tag, "Physical device " + stream.str() + " doesn't support extension named \"" + device_extensions[ i ] + "\"!" );
            return false;
        }
    }

    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures   device_features;

    vkGetPhysicalDeviceProperties( physical_device, &device_properties );
    vkGetPhysicalDeviceFeatures( physical_device, &device_features );

    uint32_t major_version( VK_VERSION_MAJOR( device_properties.apiVersion ) );
    if ( ( major_version < 1 ) &&
        ( device_properties.limits.maxImageDimension2D < 4096 ) ) {
        std::stringstream stream;
        stream << physical_device;
        osre_error( Tag, "Physical device " + stream.str() + " doesn't support required parameters!" );
        return false;
    }

    uint32_t queue_families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( physical_device, &queue_families_count, nullptr );
    if ( queue_families_count == 0 ) {
        std::stringstream stream;
        stream << physical_device;
        osre_error( Tag, "Physical device " + stream.str() + " doesn't have any queue families!" );
        return false;
    }

    TArray<VkQueueFamilyProperties>  queue_family_properties( queue_families_count );
    TArray<VkBool32>                 queue_present_support( queue_families_count );
    vkGetPhysicalDeviceQueueFamilyProperties( physical_device, &queue_families_count, &queue_family_properties[ 0 ] );
    ui32 graphics_queue_family_index = UINT32_MAX;
    ui32 present_queue_family_index = UINT32_MAX;

    for ( ui32 i = 0; i < queue_families_count; ++i ) {
        vkGetPhysicalDeviceSurfaceSupportKHR( physical_device, i, m_vulkan.m_presentationSurface, &queue_present_support[ i ] );
        if ( ( queue_family_properties[ i ].queueCount > 0 ) &&
            ( queue_family_properties[ i ].queueFlags & VK_QUEUE_GRAPHICS_BIT ) ) {
            // Select first queue that supports graphics
            if ( graphics_queue_family_index == UINT32_MAX ) {
                graphics_queue_family_index = i;
            }

            // If there is queue that supports both graphics and present - prefer it
            if ( queue_present_support[ i ] ) {
                selected_graphics_queue_family_index = i;
                selected_present_queue_family_index = i;
                return true;
            }
        }
    }

    // We don't have queue that supports both graphics and present so we have to use separate queues
    for ( ui32 i = 0; i < queue_families_count; ++i ) {
        if ( queue_present_support[ i ] ) {
            present_queue_family_index = i;
            break;
        }
    }

    // If this device doesn't support queues with graphics and present capabilities don't use it
    if ( ( graphics_queue_family_index == UINT32_MAX ) ||
        ( present_queue_family_index == UINT32_MAX ) ) {
        std::stringstream stream;
        stream << physical_device;
        osre_error( Tag, "Could not find queue families with required properties on physical device " + stream.str() + "!" );
        return false;
    }

    selected_graphics_queue_family_index = graphics_queue_family_index;
    selected_present_queue_family_index = present_queue_family_index;
    
    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
