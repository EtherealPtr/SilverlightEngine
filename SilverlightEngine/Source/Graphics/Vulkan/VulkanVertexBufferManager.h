#pragma once

#include "VulkanVertexBuffer.h"
#include "Foundation/Platform.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace Silverlight
{
	class VulkanDevice;
	class PrimitiveMeshComponent;
	class CustomMeshComponent;

	class VulkanVertexBufferManager
	{
	public:
		VulkanVertexBufferManager(const VulkanDevice& _device, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue);

		VulkanVertexBufferManager(const VulkanVertexBufferManager&) = delete;
		VulkanVertexBufferManager& operator=(const VulkanVertexBufferManager&) = delete;
		VulkanVertexBufferManager(VulkanVertexBufferManager&&) = delete;
		VulkanVertexBufferManager& operator=(VulkanVertexBufferManager&&) = delete;

		void GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData);
		void CreateBasicShapeVertexBuffer(PrimitiveMeshComponent& _meshComponent);
		void CreateModelVertexBuffer(CustomMeshComponent& _meshComponent);
		VulkanVertexBuffer* GetVertexBuffer(const uint32 _bufferId);

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkQueue m_GraphicsQueue;
		std::unordered_map<uint32, VulkanVertexBuffer> m_VertexBuffers;
		std::unordered_map<std::string, uint32> m_ModelNameToBufferId;
		static uint32 s_NextBufferId;
	};
} // End of namespace