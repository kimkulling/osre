/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

// ************************************************************ //
// Exported functions                                           //
//                                                              //
// These functions are always exposed by vulkan libraries.      //
// ************************************************************ //

#if !defined(VK_EXPORTED_FUNCTION)
#  define VK_EXPORTED_FUNCTION( fun )
#endif

VK_EXPORTED_FUNCTION( vkGetInstanceProcAddr )

#undef VK_EXPORTED_FUNCTION


// ************************************************************ //
// Global level functions                                       //
//                                                              //
// They allow checking what instance extensions are available   //
// and allow creation of a Vulkan Instance.                     //
// ************************************************************ //

#if !defined(VK_GLOBAL_LEVEL_FUNCTION)
#  define VK_GLOBAL_LEVEL_FUNCTION( fun )
#endif

VK_GLOBAL_LEVEL_FUNCTION( vkCreateInstance )

VK_GLOBAL_LEVEL_FUNCTION( vkEnumerateInstanceExtensionProperties )

#undef VK_GLOBAL_LEVEL_FUNCTION


// ************************************************************ //
// Instance level functions                                     //
//                                                              //
// These functions allow for device queries and creation.       //
// They help choose which device is well suited for our needs.  //
// ************************************************************ //

#if !defined(VK_INSTANCE_LEVEL_FUNCTION)
#  define VK_INSTANCE_LEVEL_FUNCTION( fun )
#endif

#define USE_SWAPCHAIN_EXTENSIONS

VK_INSTANCE_LEVEL_FUNCTION( vkEnumeratePhysicalDevices )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceProperties )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceFeatures )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceQueueFamilyProperties )
VK_INSTANCE_LEVEL_FUNCTION( vkCreateDevice )
VK_INSTANCE_LEVEL_FUNCTION( vkGetDeviceProcAddr )
VK_INSTANCE_LEVEL_FUNCTION( vkDestroyInstance )

VK_INSTANCE_LEVEL_FUNCTION( vkEnumerateDeviceExtensionProperties )
#if defined(USE_SWAPCHAIN_EXTENSIONS)
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfaceSupportKHR )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfaceCapabilitiesKHR )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfaceFormatsKHR )
VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceSurfacePresentModesKHR )
VK_INSTANCE_LEVEL_FUNCTION( vkDestroySurfaceKHR )
#if defined(VK_USE_PLATFORM_WIN32_KHR)
VK_INSTANCE_LEVEL_FUNCTION( vkCreateWin32SurfaceKHR )
#elif defined(VK_USE_PLATFORM_XCB_KHR)
VK_INSTANCE_LEVEL_FUNCTION( vkCreateXcbSurfaceKHR )
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
VK_INSTANCE_LEVEL_FUNCTION( vkCreateXlibSurfaceKHR )
#endif
#endif

VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceMemoryProperties )

#undef VK_INSTANCE_LEVEL_FUNCTION


// ************************************************************ //
// Device level functions                                       //
//                                                              //
// These functions are used mainly for drawing                  //
// ************************************************************ //

#if !defined(VK_DEVICE_LEVEL_FUNCTION)
#  define VK_DEVICE_LEVEL_FUNCTION( fun )
#endif

#pragma warning(push)
#pragma warning( disable : 4706 )

VK_DEVICE_LEVEL_FUNCTION( vkGetDeviceQueue )
VK_DEVICE_LEVEL_FUNCTION( vkDeviceWaitIdle )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyDevice )

VK_DEVICE_LEVEL_FUNCTION( vkCreateSemaphore )
VK_DEVICE_LEVEL_FUNCTION( vkCreateCommandPool )
VK_DEVICE_LEVEL_FUNCTION( vkAllocateCommandBuffers )
VK_DEVICE_LEVEL_FUNCTION( vkBeginCommandBuffer )
VK_DEVICE_LEVEL_FUNCTION( vkCmdPipelineBarrier )
VK_DEVICE_LEVEL_FUNCTION( vkCmdClearColorImage )
VK_DEVICE_LEVEL_FUNCTION( vkEndCommandBuffer )
VK_DEVICE_LEVEL_FUNCTION( vkQueueSubmit )
VK_DEVICE_LEVEL_FUNCTION( vkQueueWaitIdle )

VK_DEVICE_LEVEL_FUNCTION( vkFreeCommandBuffers )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyCommandPool )
VK_DEVICE_LEVEL_FUNCTION( vkDestroySemaphore )

#if defined(USE_SWAPCHAIN_EXTENSIONS)
VK_DEVICE_LEVEL_FUNCTION( vkCreateSwapchainKHR )
VK_DEVICE_LEVEL_FUNCTION( vkGetSwapchainImagesKHR )
VK_DEVICE_LEVEL_FUNCTION( vkAcquireNextImageKHR )
VK_DEVICE_LEVEL_FUNCTION( vkQueuePresentKHR )
VK_DEVICE_LEVEL_FUNCTION( vkDestroySwapchainKHR )
#endif

VK_DEVICE_LEVEL_FUNCTION( vkCreateImageView )
VK_DEVICE_LEVEL_FUNCTION( vkCreateRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkCreateFramebuffer )
VK_DEVICE_LEVEL_FUNCTION( vkCreateShaderModule )
VK_DEVICE_LEVEL_FUNCTION( vkCreatePipelineLayout )
VK_DEVICE_LEVEL_FUNCTION( vkCreateGraphicsPipelines )
VK_DEVICE_LEVEL_FUNCTION( vkCmdBeginRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkCmdBindPipeline )
VK_DEVICE_LEVEL_FUNCTION( vkCmdDraw )
VK_DEVICE_LEVEL_FUNCTION( vkCmdEndRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyShaderModule )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyPipelineLayout )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyPipeline )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyRenderPass )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyFramebuffer )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyImageView )

VK_DEVICE_LEVEL_FUNCTION( vkCreateFence )
VK_DEVICE_LEVEL_FUNCTION( vkCreateBuffer )
VK_DEVICE_LEVEL_FUNCTION( vkGetBufferMemoryRequirements )
VK_DEVICE_LEVEL_FUNCTION( vkAllocateMemory )
VK_DEVICE_LEVEL_FUNCTION( vkBindBufferMemory )
VK_DEVICE_LEVEL_FUNCTION( vkMapMemory )
VK_DEVICE_LEVEL_FUNCTION( vkUnmapMemory )
VK_DEVICE_LEVEL_FUNCTION( vkCmdSetViewport )
VK_DEVICE_LEVEL_FUNCTION( vkCmdSetScissor )
VK_DEVICE_LEVEL_FUNCTION( vkCmdBindVertexBuffers )
VK_DEVICE_LEVEL_FUNCTION( vkWaitForFences )
VK_DEVICE_LEVEL_FUNCTION( vkResetFences )
VK_DEVICE_LEVEL_FUNCTION( vkFreeMemory )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyBuffer )
VK_DEVICE_LEVEL_FUNCTION( vkDestroyFence )

#pragma warning(pop)

#undef VK_DEVICE_LEVEL_FUNCTION

