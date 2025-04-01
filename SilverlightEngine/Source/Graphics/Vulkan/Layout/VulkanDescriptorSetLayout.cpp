#include "VulkanDescriptorSetLayout.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VkDevice& _device) :
		m_Device{ _device },
		m_DescriptorSetLayout{ VK_NULL_HANDLE }
	{}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		if (m_DescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);
			m_DescriptorSetLayout = VK_NULL_HANDLE;
		}
	}
} // End of namespace