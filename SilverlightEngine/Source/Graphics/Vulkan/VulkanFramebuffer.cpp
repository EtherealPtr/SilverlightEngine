#include "VulkanFramebuffer.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
    VulkanFramebuffer::VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkImageView& _colorImageView, const VkImageView& _depthImageView, const uint32 _w, const uint32 _h) :
        m_Device{ _logicalDevice },
        m_RenderPass{ _renderPass },
        m_Framebuffer{ VK_NULL_HANDLE }
    {
        CreateFramebuffer(_w, _h, _colorImageView, _depthImageView);
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        CleanUp();
    }

    void VulkanFramebuffer::RecreateFramebuffer(const uint32 _w, const uint32 _h, const VkImageView& _colorImageView, const VkImageView& _depthImageView)
    {
        CleanUp();
        CreateFramebuffer(_w, _h, _colorImageView, _depthImageView);
    }

    void VulkanFramebuffer::CreateFramebuffer(const uint32 _w, const uint32 _h, const VkImageView& _colorImageView, const VkImageView& _depthImageView)
    {
        SE_LOG(LogCategory::Trace, "[FRAMEBUFFER]: Creating framebuffer");

        std::vector<VkImageView> attachments{};
        attachments.reserve(2);

        if (_colorImageView != VK_NULL_HANDLE)
        {
            attachments.emplace_back(_colorImageView);
        }

        if (_depthImageView != VK_NULL_HANDLE)
        {
            attachments.emplace_back(_depthImageView);
        }

        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = m_RenderPass;
        framebufferCreateInfo.attachmentCount = static_cast<uint32>(attachments.size());
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = _w;
        framebufferCreateInfo.height = _h;
        framebufferCreateInfo.layers = 1;

        if (vkCreateFramebuffer(m_Device, &framebufferCreateInfo, nullptr, &m_Framebuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR: Failed to create framebuffer");
        }

        SE_LOG(LogCategory::Info, "[FRAMEBUFFER]: Created framebuffer");
    }

    void VulkanFramebuffer::CleanUp() noexcept
    {
        vkDestroyFramebuffer(m_Device, m_Framebuffer, nullptr);
        m_Framebuffer = VK_NULL_HANDLE;
    }
} // End of namespace