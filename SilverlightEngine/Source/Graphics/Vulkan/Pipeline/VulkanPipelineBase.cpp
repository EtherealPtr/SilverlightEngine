#include "VulkanPipelineBase.h"
#include "Graphics/Vulkan/VulkanRenderContext.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanPipelineBase::VulkanPipelineBase(const VulkanRenderContext& _renderContext, const VkDescriptorSetLayout& _descriptorSetLayout, const VkRenderPass& _renderPass) noexcept :
		m_Device{ _renderContext.GetDevice().GetLogicalDevice() },
		m_DescriptorSetLayout{ _descriptorSetLayout },
		m_RenderPass{ _renderPass },
		m_PipelineLayout{ VK_NULL_HANDLE },
		m_Pipeline{ VK_NULL_HANDLE }
	{}

	VulkanPipelineBase::~VulkanPipelineBase()
	{
		vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
		m_Pipeline = VK_NULL_HANDLE;

		vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
		m_PipelineLayout = VK_NULL_HANDLE;
	}
} // End of namespace