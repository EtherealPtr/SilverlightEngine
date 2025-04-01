#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef struct VkImageView_T* VkImageView;
typedef enum VkFormat VkFormat;

namespace Silverlight
{
	class VulkanDevice;

	class VulkanDepthBuffer
	{
	public:
		VulkanDepthBuffer(const VulkanDevice& _device, const uint32 _w, const uint32 _h, const uint32 _flags);
		~VulkanDepthBuffer();

		VulkanDepthBuffer(const VulkanDepthBuffer&) = delete;
		VulkanDepthBuffer& operator=(const VulkanDepthBuffer&) = delete;
		VulkanDepthBuffer(VulkanDepthBuffer&&) = delete;
		VulkanDepthBuffer& operator=(VulkanDepthBuffer&&) = delete;

		void RecreateDepthBuffer(const uint32 _w, const uint32 _h, const uint32 _flags);
		VkFormat GetFormat() const noexcept { return m_DepthFormat; }
		const VkImageView& GetImageView() const noexcept { return m_DepthImageView; }

	private:
		void CreateDepthBuffer(const uint32 _w, const uint32 _h, const uint32 _flags);
		void CleanUp() const noexcept;

	private:
		VkDevice m_Device;
		VkPhysicalDevice m_GPU;
		VkImage m_DepthImage;
		VkImageView m_DepthImageView;
		VkDeviceMemory m_DepthImageMemory;
		VkFormat m_DepthFormat;
	};
} // End of namespace