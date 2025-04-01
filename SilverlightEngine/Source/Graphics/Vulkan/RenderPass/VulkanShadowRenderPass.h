#pragma once

#include "VulkanRenderPassBase.h"

namespace Silverlight
{
	class VulkanShadowRenderPass : public VulkanRenderPassBase
	{
	public:
		VulkanShadowRenderPass(const VkDevice& _device, const VkFormat _depthFormat);

	protected:
		virtual void CreateRenderPass() override;

	private:
		VkFormat m_DepthFormat;
	};
} // End of namespace