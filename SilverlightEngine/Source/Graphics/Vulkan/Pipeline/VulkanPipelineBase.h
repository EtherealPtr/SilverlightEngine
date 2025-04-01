#pragma once

typedef struct VkDevice_T* VkDevice;
typedef struct VkPipelineLayout_T* VkPipelineLayout;
typedef struct VkPipeline_T* VkPipeline;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;
typedef struct VkRenderPass_T* VkRenderPass;

namespace Silverlight
{
	class VulkanRenderContext;

	class VulkanPipelineBase
	{
	public:
		VulkanPipelineBase(const VulkanRenderContext& _renderContext, const VkDescriptorSetLayout& _descriptorSetLayout, const VkRenderPass& _renderPass) noexcept;
		virtual ~VulkanPipelineBase();

		VulkanPipelineBase(const VulkanPipelineBase&) = delete;
		VulkanPipelineBase& operator=(const VulkanPipelineBase&) = delete;
		VulkanPipelineBase(VulkanPipelineBase&&) = delete;
		VulkanPipelineBase& operator=(VulkanPipelineBase&&) = delete;

		const VkPipeline& Get() const noexcept { return m_Pipeline; }
		const VkPipelineLayout& GetLayout() const noexcept { return m_PipelineLayout; }

	protected:
		virtual void CreatePipeline() = 0;

	protected:
		VkDevice m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkRenderPass m_RenderPass;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_Pipeline;
	};
} // End of namespace