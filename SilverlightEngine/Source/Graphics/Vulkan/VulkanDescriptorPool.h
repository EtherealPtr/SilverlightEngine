#pragma once

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorPool_T* VkDescriptorPool;

namespace Silverlight
{
	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorPool(const VkDevice& _logicalDevice, const size_t _maxSets);
		~VulkanDescriptorPool();

		VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
		VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;
		VulkanDescriptorPool(VulkanDescriptorPool&&) = delete;
		VulkanDescriptorPool& operator=(VulkanDescriptorPool&&) = delete;

		const VkDescriptorPool& Get() const noexcept { return m_DescriptorPool; }

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorPool m_DescriptorPool;
	};
} // End of namespace