#include "VulkanDescriptorSetLayout.h"
#include "Foundation/Logging/Logger.h"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VkDevice& _device, const std::span<const DescriptorSetLayoutBinding> _bindings) :
		m_Device{ _device },
		m_DescriptorSetLayout{ VK_NULL_HANDLE }
	{
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
		layoutBindings.reserve(_bindings.size());

		for (auto const& binding : _bindings)
		{
			VkDescriptorSetLayoutBinding layoutBinding{};
			layoutBinding.binding = binding.m_Binding;
			layoutBinding.descriptorType = static_cast<VkDescriptorType>(binding.m_DescriptorType);
			layoutBinding.descriptorCount = binding.m_DescriptorCount;
			layoutBinding.stageFlags = static_cast<VkShaderStageFlags>(binding.m_StageFlags);
			layoutBinding.pImmutableSamplers = nullptr;
			layoutBindings.emplace_back(layoutBinding);
		}

		VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutCreateInfo.bindingCount = static_cast<uint32>(layoutBindings.size());
		layoutCreateInfo.pBindings = layoutBindings.data();

		if (vkCreateDescriptorSetLayout(m_Device, &layoutCreateInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a descriptor set layout");
		}

		SE_LOG(LogCategory::Info, "[DESCRIPTOR SET LAYOUT]: Created a descriptor set layout");
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		if (m_DescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);
			m_DescriptorSetLayout = VK_NULL_HANDLE;
		}
	}
} // End of namespace