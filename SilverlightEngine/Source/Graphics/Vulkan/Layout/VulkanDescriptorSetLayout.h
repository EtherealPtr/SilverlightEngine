#pragma once

#include "Foundation/Platform.h"
#include <span>

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Silverlight
{
	struct DescriptorSetLayoutBinding
	{
		uint32 m_Binding{ 0 };
		uint32 m_DescriptorType{ 0 };
		uint32 m_DescriptorCount{ 0 };
		uint32 m_StageFlags{ 0 };
	};

	class VulkanDescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout(const VkDevice& _device, const std::span<const DescriptorSetLayoutBinding> _bindings);
		~VulkanDescriptorSetLayout();

		VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) = delete;
		VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&&) = delete;

		const VkDescriptorSetLayout& Get() const noexcept { return m_DescriptorSetLayout; }

	private:
		VkDevice m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
} // End of namespace