#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkSemaphore_T* VkSemaphore;

namespace Silverlight
{
	class VulkanSemaphore
	{
	public:
		VulkanSemaphore(const VkDevice& _logicalDevice, const uint16 _count = 1);
		~VulkanSemaphore();

		VulkanSemaphore(const VulkanSemaphore&) = delete;
		VulkanSemaphore& operator=(const VulkanSemaphore&) = delete;
		VulkanSemaphore(VulkanSemaphore&&) = delete;
		VulkanSemaphore& operator=(VulkanSemaphore&&) = delete;

		const std::vector<std::pair<VkSemaphore, VkSemaphore>>& Get() noexcept { return m_Semaphores; }

	private:
		VkDevice m_LogicalDevice;
		std::vector<std::pair<VkSemaphore, VkSemaphore>> m_Semaphores;
	};
} // End of namespace