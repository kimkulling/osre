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

namespace OSRE {
namespace RenderBackend {

VlkRenderBackend::VlkRenderBackend()
: m_vulkan() {
    // empty
}

VlkRenderBackend::~VlkRenderBackend() {
    // empty
}

VkDevice VlkRenderBackend::getDevice() const {
    return m_vulkan.m_device;
}

const VlkSwapChainParameters VlkRenderBackend::getSwapChain() const {
    return m_vulkan.m_swapChain;
}

bool VlkRenderBackend::createRenderPass() {
    VkAttachmentDescription attachment_descriptions[] = {
        {
            0,                                          // VkAttachmentDescriptionFlags   flags
            getSwapChain().m_format,                      // VkFormat                       format
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
