#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;

namespace Silverlight
{
	class VulkanUniformBuffer
	{
	public:
		VulkanUniformBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint64 _size, const uint32 _usage);
		~VulkanUniformBuffer();

		void CopyData(void* _pData) const noexcept;
		const VkBuffer& GetBuffer() const noexcept { return m_Buffer; }
		uint64 GetBufferSize() const noexcept { return m_BufferSize; }
		const VkDeviceMemory& GetBufferMemory() const noexcept { return m_BufferMemory; }

	private:
		VkDevice m_LogicalDevice;
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		uint64 m_BufferSize;
	};
} // End of namespace