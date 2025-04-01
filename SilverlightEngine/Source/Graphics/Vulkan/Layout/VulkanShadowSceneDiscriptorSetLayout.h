#pragma once

#include "VulkanDescriptorSetLayout.h"

namespace Silverlight
{
	class VulkanShadowSceneDiscriptorSetLayout : public VulkanDescriptorSetLayout
	{
	public:
		explicit VulkanShadowSceneDiscriptorSetLayout(const VkDevice& _device);

	protected:
		virtual void CreateDescriptorSetLayout() override;
	};
} // End of namespace