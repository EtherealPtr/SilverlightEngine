#include "VulkanRenderPassBase.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanRenderPassBase::VulkanRenderPassBase(const VkDevice& _device) noexcept :
		m_Device{ _device },
		m_RenderPass{ VK_NULL_HANDLE }
	{}

	void VulkanRenderPassBase::CleanUp() noexcept
	{
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
		m_RenderPass = VK_NULL_HANDLE;
	}
} // End of namespace