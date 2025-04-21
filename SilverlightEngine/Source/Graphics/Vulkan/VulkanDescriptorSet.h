#pragma once

#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorPool_T* VkDescriptorPool;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;
typedef struct VkDescriptorSet_T* VkDescriptorSet;

namespace Silverlight
{
	struct VulkanBufferDescriptorInfo;
	struct VulkanTextureDescriptorInfo;

	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet(const VkDevice& _logicalDevice, const VkDescriptorPool& _descriptorPool, const VkDescriptorSetLayout& _descriptorLayout);
		~VulkanDescriptorSet();

		void UpdateDescriptorSet(const VulkanBufferDescriptorInfo& _bufferInfo) const noexcept;
		void UpdateDescriptorSet(const VulkanTextureDescriptorInfo& _textureInfo) const noexcept;
		const VkDescriptorSet& Get() const noexcept { return m_DescriptorSet; }

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSet;
	};
} // End of namespace