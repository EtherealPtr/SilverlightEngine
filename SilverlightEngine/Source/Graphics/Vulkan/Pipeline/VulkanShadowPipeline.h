#pragma once

#include "Foundation/Platform.h"
#include "VulkanPipelineBase.h"

namespace Silverlight
{
	class VulkanRenderContext;

	class VulkanShadowPipeline : public VulkanPipelineBase
	{
	public:
		VulkanShadowPipeline(const VulkanRenderContext& _renderContext, const VkDescriptorSetLayout& _descriptorSetLayout, const VkRenderPass& _renderPass, const uint32 _width, const uint32 _height);

	protected:
		virtual void CreatePipeline() override;

	private:
		uint32 m_Width;
		uint32 m_Height;
	};
} // End of namespace