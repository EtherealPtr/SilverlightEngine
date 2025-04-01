#include "VulkanCommandPool.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanCommandPool::VulkanCommandPool(const VkDevice& _logicalDevice, const uint32 _queueFamilyIndex) :
		m_LogicalDevice{ _logicalDevice },
		m_CommandPool{ VK_NULL_HANDLE }
	{
		SE_LOG(LogCategory::Trace, "[COMMAND POOL]: Creating command pool");

		VkCommandPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolCreateInfo.queueFamilyIndex = _queueFamilyIndex;

		if (vkCreateCommandPool(_logicalDevice, &poolCreateInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a command pool");
		}

		SE_LOG(LogCategory::Info, "[COMMAND POOL]: Created command pool");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
		m_CommandPool = VK_NULL_HANDLE;
	}
}
