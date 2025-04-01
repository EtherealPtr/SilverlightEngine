#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkQueue_T* VkQueue;
typedef struct VkSemaphore_T* VkSemaphore;
typedef struct VkFence_T* VkFence;

namespace Silverlight
{
	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _pool, const uint16 _count = 1);
		~VulkanCommandBuffer();

		VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
		VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;
		VulkanCommandBuffer(VulkanCommandBuffer&&) = delete;
		VulkanCommandBuffer& operator=(VulkanCommandBuffer&&) = delete;

		void Begin(const uint16 _bufferIndex = 0) const noexcept;
		void End(const uint16 _bufferIndex = 0) const noexcept;
		void Submit(const uint16 _bufferIndex, const VkQueue& _queue, const VkSemaphore& _waitSem = nullptr, const VkSemaphore& _signalSem = nullptr, const VkFence& _fence = nullptr, const uint32 _waitStage = 0);
		const std::vector<VkCommandBuffer>& Get() const noexcept { return m_CommandBuffers; }

	private:
		VkDevice m_LogicalDevice;
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
} // End of namespace