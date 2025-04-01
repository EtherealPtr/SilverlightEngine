#include "VulkanCommandBuffer.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanCommandBuffer::VulkanCommandBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _pool, const uint16 _count) :
		m_LogicalDevice{ _logicalDevice },
		m_CommandPool{ _pool },
		m_CommandBuffers{ _count, VK_NULL_HANDLE }
	{
		SE_LOG(LogCategory::Trace, "[COMMAND BUFFER]: Creating command buffer");

		VkCommandBufferAllocateInfo commandBufferAllocInfo{};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.commandPool = _pool;
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocInfo.commandBufferCount = _count;

		if (vkAllocateCommandBuffers(_logicalDevice, &commandBufferAllocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to allocate command buffers");
		}

		SE_LOG(LogCategory::Info, "[COMMAND BUFFER]: Created command buffer");
	}

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		vkFreeCommandBuffers(m_LogicalDevice, m_CommandPool, static_cast<uint32>(m_CommandBuffers.size()), m_CommandBuffers.data());
	}

	void VulkanCommandBuffer::Begin(const uint16 _bufferIndex) const noexcept
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		vkBeginCommandBuffer(m_CommandBuffers.at(_bufferIndex), &beginInfo);
	}

	void VulkanCommandBuffer::End(const uint16 _bufferIndex) const noexcept
	{
		vkEndCommandBuffer(m_CommandBuffers.at(_bufferIndex));
	}

	void VulkanCommandBuffer::Submit(const uint16 _bufferIndex, const VkQueue& _queue, const VkSemaphore& _waitSem, const VkSemaphore& _signalSem, const VkFence& _fence, const uint32 _waitStage)
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = _waitSem ? 1 : 0;
		submitInfo.pWaitSemaphores = &_waitSem;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers.at(_bufferIndex);
		submitInfo.signalSemaphoreCount = _signalSem ? 1 : 0;
		submitInfo.pSignalSemaphores = &_signalSem;
		submitInfo.pWaitDstStageMask = &_waitStage;

		if (vkQueueSubmit(_queue, 1, &submitInfo, _fence) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to submit command buffer to queue");
		}
	}
} // End of namespace