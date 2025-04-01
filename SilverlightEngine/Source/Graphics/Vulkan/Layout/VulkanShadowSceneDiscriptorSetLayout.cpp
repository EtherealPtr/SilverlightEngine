#include "VulkanShadowSceneDiscriptorSetLayout.h"
#include "Foundation/Logging/Logger.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanShadowSceneDiscriptorSetLayout::VulkanShadowSceneDiscriptorSetLayout(const VkDevice& _device) :
		VulkanDescriptorSetLayout{ _device }
	{
		CreateDescriptorSetLayout();
	}

	void VulkanShadowSceneDiscriptorSetLayout::CreateDescriptorSetLayout()
	{
		std::array<VkDescriptorSetLayoutBinding, 1> layoutBindings =
		{
			VkDescriptorSetLayoutBinding { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr }
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