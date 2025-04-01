#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkFence_T* VkFence;

namespace Silverlight
{
	class VulkanFence
	{
	public:
		VulkanFence(const VkDevice& _logicalDevice, const uint16 _count = 1);
		~VulkanFence();

		VulkanFence(const VulkanFence&) = delete;
		VulkanFence& operator=(const VulkanFence&) = delete;
		VulkanFence(VulkanFence&&) = delete;
		VulkanFence& operator=(VulkanFence&&) = delete;

		void Wait(const uint16 _fenceIndex = 1) const noexcept;
		void Reset(const uint16 _fenceIndex = 1) const noexcept;
		const std::vector<VkFence>& Get() const noexcept { return m_Fences; }

	private:
		VkDevice m_LogicalDevice;
		std::vector<VkFence> m_Fences;
	};
} // End of namespace