#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorInfo.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanDescriptorSet::VulkanDescriptorSet(const VkDevice& _logicalDevice, const VkDescriptorPool& _descriptorPool, const VkDescriptorSetLayout& _descriptorLayout) :
		m_LogicalDevice{ _logicalDevice },
		m_DescriptorPool{ _descriptorPool },
		m_DescriptorSet{ VK_NULL_HANDLE }
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &_descriptorLayout;

		if (vkAllocateDescriptorSets(_logicalDevice, &allocInfo, &m_DescriptorSet) != VK_SUCCESS)
		{
			SE_LOG(LogCategory::Warning, "[DESCRIPTOR SET]: Failed to allocate a descriptor set");
		}
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		vkFreeDescriptorSets(m_LogicalDevice, m_DescriptorPool, 1, &m_DescriptorSet);
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(const VulkanBufferDescriptorInfo& _bufferInfo) const noexcept
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.offset = 0;
		bufferInfo.buffer = _bufferInfo.m_Buffer;
		bufferInfo.range = _bufferInfo.m_Size;

		VkWriteDescriptorSet setWriter{};
		setWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		setWriter.dstSet = m_DescriptorSet;
		setWriter.dstBinding = _bufferInfo.m_Binding;
		setWriter.descriptorCount = 1;
		setWriter.descriptorType = _bufferInfo.m_Type;
		setWriter.pBufferInfo = &bufferInfo;
		setWriter.pImageInfo = nullptr;

		vkUpdateDescriptorSets(m_LogicalDevice, 1, &setWriter, 0, nullptr);
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(const VulkanTextureDescriptorInfo& _textureInfo) const noexcept
	{
		VkWriteDescriptorSet setWriter{};
		setWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		setWriter.dstSet = m_DescriptorSet;
		setWriter.dstBinding = _textureInfo.m_Binding;
		setWriter.descriptorType = _textureInfo.m_Type;

		std::vector<VkDescriptorImageInfo> imageInfos{};
		VkDescriptorImageInfo samplerInfo{};

		if (_textureInfo.m_Type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			imageInfos.reserve(_textureInfo.m_ImageViews.size());

			for (const VkImageView& imgView : _textureInfo.m_ImageViews)
			{
				VkDescriptorImageInfo imgDescriptor{};
				imgDescriptor.imageLayout = _textureInfo.m_ImageLayout;
				imgDescriptor.imageView = imgView;
				imgDescriptor.sampler = _textureInfo.m_Sampler;
				imageInfos.emplace_back(imgDescriptor);
			}

			setWriter.descriptorCount = static_cast<uint32>(_textureInfo.m_ImageViews.size());
			setWriter.pImageInfo = imageInfos.data();
		}
		else if (_textureInfo.m_Type == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
		{
			imageInfos.reserve(_textureInfo.m_ImageViews.size());

			for (const VkImageView& imgView : _textureInfo.m_ImageViews)
			{
				VkDescriptorImageInfo imgDescriptor{};
				imgDescriptor.imageLayout = _textureInfo.m_ImageLayout;
				imgDescriptor.imageView = imgView;
				imgDescriptor.sampler = VK_NULL_HANDLE;
				imageInfos.emplace_back(imgDescriptor);
			}

			setWriter.descriptorCount = static_cast<uint32>(_textureInfo.m_ImageViews.size());
			setWriter.pImageInfo = imageInfos.data();
		}
		else
		{
			samplerInfo.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			samplerInfo.imageView = VK_NULL_HANDLE;
			samplerInfo.sampler = _textureInfo.m_Sampler;
			setWriter.descriptorCount = 1;
			setWriter.pImageInfo = &samplerInfo;
		}

		vkUpdateDescriptorSets(m_LogicalDevice, 1, &setWriter, 0, nullptr);
	}
} // End of namespace