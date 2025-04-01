#pragma once

#include "VulkanRenderPassBase.h"

namespace Silverlight
{
	class VulkanSceneRenderPass : public VulkanRenderPassBase
	{
	public:
		VulkanSceneRenderPass(const VkDevice& _device, const VkFormat _colorFormat, const VkFormat _depthFormat);

	protected:
		virtual void CreateRenderPass() override;

	private:
		VkFormat m_ColorFormat;
		VkFormat m_DepthFormat;
	};
} // End of namespace