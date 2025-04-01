#include "VulkanUniformBuffer.h"
#include "VulkanUtils.h"
#include <vulkan/vulkan_core.h>
#include <stdexcept>

namespace Silverlight
{
	VulkanUniformBuffer::VulkanUniformBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint64 _size, const uint32 _usage) :
		m_LogicalDevice{ _logicalDevice },
		m_Buffer{ VK_NULL_HANDLE },
		m_BufferMemory{ VK_NULL_HANDLE },
		m_BufferSize{ _size }
	{
		// Create buffer object
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = _size;
		bufferCreateInfo.usage = _usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(_logicalDevice, &bufferCreateInfo, nullptr, &m_Buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a uniform buffer object");
		}

		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(_logicalDevice, m_Buffer, &memRequirements);

		// Allocate memory for the buffer
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanUtils::FindMemoryTypeIndex(_gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		vkAllocateMemory(_logicalDevice, &allocInfo, nullptr, &m_BufferMemory);
		vkBindBufferMemory(_logicalDevice, m_Buffer, m_BufferMemory, 0);
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		vkFreeMemory(m_LogicalDevice, m_BufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, m_Buffer, nullptr);
		m_BufferMemory = VK_NULL_HANDLE;
		m_Buffer = VK_NULL_HANDLE;
	}

	void VulkanUniformBuffer::CopyData(void* _pData) const noexcept
	{
		// Map the memory
		void* mappedData{ nullptr };
		vkMapMemory(m_LogicalDevice, m_BufferMemory, 0, m_BufferSize, 0, &mappedData);

		// Copy the data to the buffer
		memcpy(mappedData, _pData, m_BufferSize);

		// Unmap the memory
		vkUnmapMemory(m_LogicalDevice, m_BufferMemory);
	}
} // End of namespace