#include "VulkanFence.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanFence::VulkanFence(const VkDevice& _logicalDevice, const uint16 _count) :
		m_LogicalDevice{ _logicalDevice },
		m_Fences{ VK_NULL_HANDLE }
	{
		SE_LOG(LogCategory::Trace, "[FENCE]: Creating %d fences", _count);

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_Fences.resize(_count);

		for (size_t i = 0; i < _count; ++i)
		{
			if (vkCreateFence(_logicalDevice, &fenceCreateInfo, nullptr, &m_Fences.at(i)) != VK_SUCCESS)
			{
				throw std::runtime_error("ERROR: Failed to create fence\n");
			}
		}

		SE_LOG(LogCategory::Info, "[FENCE]: Created fences");
	}

	VulkanFence::~VulkanFence()
	{
		for (size_t i = 0; i < m_Fences.size(); ++i)
		{
			vkDestroyFence(m_LogicalDevice, m_Fences.at(i), nullptr);
			m_Fences.at(i) = VK_NULL_HANDLE;
		}
	}

	void VulkanFence::Wait(const uint16 _fenceIndex) const noexcept
	{
		vkWaitForFences(m_LogicalDevice, 1, &m_Fences.at(_fenceIndex), VK_TRUE, UINT64_MAX);
	}

	void VulkanFence::Reset(const uint16 _fenceIndex) const noexcept
	{
		vkResetFences(m_LogicalDevice, 1, &m_Fences.at(_fenceIndex));
	}
} // End of namespace