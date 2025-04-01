#pragma once

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Silverlight
{
	class VulkanDescriptorSetLayout
	{
	public:
		explicit VulkanDescriptorSetLayout(const VkDevice& _device);
		virtual ~VulkanDescriptorSetLayout();

		VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) = delete;
		VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&&) = delete;

		const VkDescriptorSetLayout& Get() const noexcept { return m_DescriptorSetLayout; }

	protected:
		virtual void CreateDescriptorSetLayout() = 0;

	protected:
		VkDevice m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
} // End of namespace