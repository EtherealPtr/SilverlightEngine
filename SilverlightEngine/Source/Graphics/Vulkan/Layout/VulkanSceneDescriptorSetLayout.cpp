#include "VulkanSceneDescriptorSetLayout.h"
#include "Foundation/Logging/Logger.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanSceneDescriptorSetLayout::VulkanSceneDescriptorSetLayout(const VkDevice& _device) :
		VulkanDescriptorSetLayout{ _device }
	{
		CreateDescriptorSetLayout();
	}

	void VulkanSceneDescriptorSetLayout::CreateDescriptorSetLayout()
	{
		std::array<VkDescriptorSetLayoutBinding, 7> layoutBindings =
		{
			VkDescriptorSetLayoutBinding { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
			VkDescriptorSetLayoutBinding { 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			VkDescriptorSetLayoutBinding { 2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 16, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			VkDescriptorSetLayoutBinding { 3, VK_DESCRIPTOR_TYPE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			VkDescriptorSetLayoutBinding { 4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			VkDescriptorSetLayoutBinding { 5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
			VkDescriptorSetLayoutBinding { 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }
		};

		VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutCreateInfo.bindingCount = static_cast<uint32>(layoutBindings.size());
		layoutCreateInfo.pBindings = layoutBindings.data();

		if (vkCreateDescriptorSetLayout(m_Device, &layoutCreateInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create descriptor set layout");
		}

		SE_LOG(LogCategory::Info, "[DESCRIPTOR SET LAYOUT]: Created descriptor set layout");
	}
} // End of namespace