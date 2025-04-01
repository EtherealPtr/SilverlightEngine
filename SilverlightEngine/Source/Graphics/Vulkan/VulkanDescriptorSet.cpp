#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorSetWriters.h"
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
			SE_LOG(LogCategory::Warning, "[DESCRIPTOR SET]: Failed to allocate descriptor sets");
		}
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		vkFreeDescriptorSets(m_LogicalDevice, m_DescriptorPool, 1, &m_DescriptorSet);
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(const std::vector<VulkanDescriptorSetBufferWriter>& _descriptorSetBufferWriters) const noexcept
	{
		for (const auto& writer : _descriptorSetBufferWriters)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.offset = 0;
			bufferInfo.buffer = writer.GetBuffer();
			bufferInfo.range = writer.GetBufferRange();

			VkWriteDescriptorSet descriptorSetWriter{};
			descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorSetWriter.dstSet = m_DescriptorSet;
			descriptorSetWriter.dstBinding = writer.GetBinding();
			descriptorSetWriter.descriptorCount = 1;
			descriptorSetWriter.descriptorType = writer.GetDescriptorType();
			descriptorSetWriter.pBufferInfo = &bufferInfo;
			descriptorSetWriter.pImageInfo = nullptr;

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorSetWriter, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(const std::vector<VulkanDescriptorSetTextureWriter>& _descriptorSetTextureWriters)
	{
		for (const auto& writer : _descriptorSetTextureWriters)
		{
			const VkImageLayout imageLayout{ (writer.GetDescriptorType() == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER) ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

			std::vector<VkDescriptorImageInfo> imageInfos{};
			imageInfos.reserve(writer.GetImageViews().size());

			if (writer.GetImageViews().empty())
			{
				imageInfos.emplace_back(writer.GetSampler(), VK_NULL_HANDLE, imageLayout);
			}
			else
			{
				for (const auto& imageView : writer.GetImageViews())
				{
					imageInfos.emplace_back(writer.GetDescriptorType() == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ? writer.GetSampler() : VK_NULL_HANDLE, imageView, imageLayout);
				}
			}

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSet;
			descriptorWrite.dstBinding = writer.GetBinding();
			descriptorWrite.descriptorCount = static_cast<uint32>(imageInfos.size());
			descriptorWrite.descriptorType = writer.GetDescriptorType();
			descriptorWrite.pImageInfo = imageInfos.data();

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorWrite, 0, nullptr);
		}
	}
} // End of namespace