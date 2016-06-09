#pragma once

#include <osre/Common/osre_common.h>

#include "vulkan.h"

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

#define VK_EXPORTED_FUNCTION( fun ) extern PFN_##fun fun;
#define VK_GLOBAL_LEVEL_FUNCTION( fun) extern PFN_##fun fun;
#define VK_INSTANCE_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;
#define VK_DEVICE_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;

}
}
