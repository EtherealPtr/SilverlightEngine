#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkImageView_T* VkImageView;
typedef struct VkSampler_T* VkSampler;
typedef enum VkDescriptorType VkDescriptorType;
typedef enum VkImageLayout VkImageLayout;

namespace Silverlight
{
	struct VulkanBufferDescriptorInfo
	{
		uint32 m_Binding;
		VkDescriptorType m_Type;
		VkBuffer m_Buffer;
		uint64 m_Size;
	};
	
	struct VulkanTextureDescriptorInfo
	{
		uint32 m_Binding;
		VkDescriptorType m_Type;
		std::vector<VkImageView> m_ImageViews;
		VkSampler m_Sampler;
		VkImageLayout m_ImageLayout;
	};
} // End of namespace