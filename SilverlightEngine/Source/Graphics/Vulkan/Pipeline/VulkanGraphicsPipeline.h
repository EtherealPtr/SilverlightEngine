#pragma once

#include "Foundation/Platform.h"
#include "VulkanPipelineBase.h"

namespace Silverlight
{
	class VulkanRenderContext;

	class VulkanGraphicsPipeline : public VulkanPipelineBase
	{
	public:
		VulkanGraphicsPipeline(const VulkanRenderContext& _renderContext, const VkDescriptorSetLayout& _descriptorSetLayout, const VkRenderPass& _renderPass);

	protected:
		virtual void CreatePipeline() override;

	private:
		uint32 m_Width;
		uint32 m_Height;
	};
} // End of namespace