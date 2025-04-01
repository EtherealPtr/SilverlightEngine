#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkQueue_T* VkQueue;

namespace Silverlight
{
	class VulkanVertexBuffer
	{
	public:
		VulkanVertexBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData);
		~VulkanVertexBuffer();

		void Bind(const VkCommandBuffer& _commandBuffer, const uint64 _indexOffset) const noexcept;

		VulkanVertexBuffer(const VulkanVertexBuffer&) = delete;
		VulkanVertexBuffer& operator=(const VulkanVertexBuffer&) = delete;
		VulkanVertexBuffer(VulkanVertexBuffer&&) = delete;
		VulkanVertexBuffer& operator=(VulkanVertexBuffer&&) = delete;

	private:
		void CreateVertexBuffer(void* _data, const uint64 _size);
		void CreateIndexBuffer(void* _data, const uint64 _size);
		void CleanUpVertexBuffer() noexcept;
		void CleanUpIndexBuffer() noexcept;

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkQueue m_GraphicsQueue;
		VkBuffer m_VertexBuffer;
		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		VkDeviceMemory m_IndexBufferMemory;
	};
} // End of namespace