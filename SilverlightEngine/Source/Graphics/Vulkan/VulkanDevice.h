#pragma once

#include "Foundation/Platform.h"

typedef struct VkInstance_T* VkInstance;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
typedef struct VkQueue_T* VkQueue;
struct VkPhysicalDeviceProperties;
struct VkPhysicalDeviceLimits;

namespace Silverlight
{
	struct DeviceQueueIndices
	{
		bool Validate() const noexcept
		{
			return (m_GraphicsQueueFamilyIndex != UINT32_MAX) &&
				   (m_TransferQueueFamilyIndex != UINT32_MAX) &&
				   (m_PresentationQueueFamilyIndex != UINT32_MAX);
		}

		uint32 m_GraphicsQueueFamilyIndex{ UINT32_MAX };
		uint32 m_TransferQueueFamilyIndex{ UINT32_MAX };
		uint32 m_PresentationQueueFamilyIndex{ UINT32_MAX };
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(const VkInstance& _vkInstance, const VkSurfaceKHR& _vkSurface);
		~VulkanDevice();

		VulkanDevice(const VulkanDevice&) = delete;
		VulkanDevice& operator=(const VulkanDevice&) = delete;
		VulkanDevice(VulkanDevice&&) = delete;
		VulkanDevice& operator=(VulkanDevice&&) = delete;

		const VkPhysicalDevice& GetPhysicalDevice() const noexcept { return m_PhysicalDevice; }
		const VkDevice& GetLogicalDevice() const noexcept { return m_LogicalDevice; }
		const DeviceQueueIndices& GetQueueIndices() const noexcept { return m_QueueIndices; }
		const VkQueue& GetGraphicsQueue() const noexcept { return m_GraphicsQueue; }
		const VkQueue& GetPresentationQueue() const noexcept { return m_PresentQueue; }
		const VkQueue& GetTransferQueue() const noexcept { return m_TransferQueue; }
		VkPhysicalDeviceLimits GetLimits() const noexcept;

	private:
		void PickPhysicalDevice(const VkInstance& _vkInstance);
		bool CheckDeviceFeatures(const VkPhysicalDevice& _gpu) const noexcept;
		uint32 RateDeviceSuitability(const VkPhysicalDevice& _gpu, const VkPhysicalDeviceProperties& _deviceProperties);
		void SetupQueueFamilyIndices();
		void CreateLogicalDevice();

	private:
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;
		DeviceQueueIndices m_QueueIndices;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_TransferQueue;
		VkQueue m_PresentQueue;
	};
} // End of namespace