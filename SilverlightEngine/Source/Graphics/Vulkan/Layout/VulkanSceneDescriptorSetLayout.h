#pragma once

#include "VulkanDescriptorSetLayout.h"

namespace Silverlight
{
	class VulkanSceneDescriptorSetLayout : public VulkanDescriptorSetLayout
	{
	public:
		explicit VulkanSceneDescriptorSetLayout(const VkDevice& _device);

	protected:
		virtual void CreateDescriptorSetLayout() override;
	};
} // End of namespace