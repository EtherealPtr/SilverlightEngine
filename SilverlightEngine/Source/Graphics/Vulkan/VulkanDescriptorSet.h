#pragma once

#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorPool_T* VkDescriptorPool;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;
typedef struct VkDescriptorSet_T* VkDescriptorSet;

namespace Silverlight
{
	class VulkanDescriptorSetBufferWriter;
	class VulkanDescriptorSetTextureWriter;

	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet(const VkDevice& _logicalDevice, const VkDescriptorPool& _descriptorPool, const VkDescriptorSetLayout& _descriptorLayout);
		~VulkanDescriptorSet();

		void UpdateDescriptorSet(const std::vector<VulkanDescriptorSetBufferWriter>& _descriptorSetBufferWriters) const noexcept;
		void UpdateDescriptorSet(const std::vector<VulkanDescriptorSetTextureWriter>& _descriptorSetTextureWriters);
		const VkDescriptorSet& Get() const noexcept { return m_DescriptorSet; }

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSet;
	};
} // End of namespace