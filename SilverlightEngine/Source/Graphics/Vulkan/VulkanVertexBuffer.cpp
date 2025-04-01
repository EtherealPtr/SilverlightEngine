#include "VulkanVertexBuffer.h"
#include "VulkanUtils.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanVertexBuffer::VulkanVertexBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue,
		void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData) :
		m_LogicalDevice{ _logicalDevice },
		m_PhysicalDevice{ _physicalDevice },
		m_CommandPool{ _commandPool },
		m_GraphicsQueue{ _graphicsQueue },
		m_VertexBuffer{ VK_NULL_HANDLE },
		m_IndexBuffer{ VK_NULL_HANDLE },
		m_VertexBufferMemory{ VK_NULL_HANDLE },
		m_IndexBufferMemory{ VK_NULL_HANDLE }
	{
		CreateVertexBuffer(_vertexData, _sizeOfVertexData);
		CreateIndexBuffer(_indexData, _sizeOfIndexData);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		CleanUpVertexBuffer();
		CleanUpIndexBuffer();
	}

	void VulkanVertexBuffer::Bind(const VkCommandBuffer& _commandBuffer, const uint64 _indexOffset) const noexcept
	{
		const VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &m_VertexBuffer, offsets);
		vkCmdBindIndexBuffer(_commandBuffer, m_IndexBuffer, _indexOffset, VK_INDEX_TYPE_UINT32);
	}

	void VulkanVertexBuffer::CreateVertexBuffer(void* _data, const uint64 _size)
	{
		// Create staging buffer
		VkBuffer stagingBuffer{};
		VkDeviceMemory stagingBufferMemory{};

		VulkanUtils::CreateBuffer(m_LogicalDevice, m_PhysicalDevice, _size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		// Map the memory
		void* mappedData{ nullptr };
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, _size, 0, &mappedData);

		// Copy the vertex data to the staging buffer
		memcpy(mappedData, _data, _size);

		// Unmap the memory
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Create vertex buffer
		VulkanUtils::CreateBuffer(m_LogicalDevice, m_PhysicalDevice, _size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

		// Copy content of staging buffer into vertex buffer
		VulkanUtils::CopyBuffer(m_LogicalDevice, m_CommandPool, m_GraphicsQueue, _size, stagingBuffer, m_VertexBuffer);

		// Clean up staging buffers
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		stagingBufferMemory = VK_NULL_HANDLE;
		stagingBuffer = VK_NULL_HANDLE;
	}

	void VulkanVertexBuffer::CreateIndexBuffer(void* _data, const uint64 _size)
	{
		// Create staging buffer
		VkBuffer stagingBuffer{};
		VkDeviceMemory stagingBufferMemory{};

		VulkanUtils::CreateBuffer(m_LogicalDevice, m_PhysicalDevice, _size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		// Map the memory
		void* mappedData{ nullptr };
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, _size, 0, &mappedData);

		// Copy the index data to the staging buffer
		memcpy(mappedData, _data, _size);

		// Unmap the memory
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		// Create vertex buffer
		VulkanUtils::CreateBuffer(m_LogicalDevice, m_PhysicalDevice, _size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

		// Copy content of staging buffer into index buffer
		VulkanUtils::CopyBuffer(m_LogicalDevice, m_CommandPool, m_GraphicsQueue, _size, stagingBuffer, m_IndexBuffer);

		// Clean up staging buffers
		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		stagingBufferMemory = VK_NULL_HANDLE;
		stagingBuffer = VK_NULL_HANDLE;
	}

	void VulkanVertexBuffer::CleanUpVertexBuffer() noexcept
	{
		vkFreeMemory(m_LogicalDevice, m_VertexBufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, m_VertexBuffer, nullptr);
		m_VertexBufferMemory = VK_NULL_HANDLE;
		m_VertexBuffer = VK_NULL_HANDLE;
	}

	void VulkanVertexBuffer::CleanUpIndexBuffer() noexcept
	{
		vkFreeMemory(m_LogicalDevice, m_IndexBufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, m_IndexBuffer, nullptr);
		m_IndexBufferMemory = VK_NULL_HANDLE;
		m_IndexBuffer = VK_NULL_HANDLE;
	}
} // End of namespace