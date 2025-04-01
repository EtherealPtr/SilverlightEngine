#include "VulkanSwapchain.h"
#include "VulkanUtils.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>
#include <algorithm>

namespace Silverlight
{
	static VkSurfaceFormatKHR PickSurfaceFormat(const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface)
	{
		// Query available surface formats
		uint32 formatCount{ 0 };
		vkGetPhysicalDeviceSurfaceFormatsKHR(_gpu, _surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_gpu, _surface, &formatCount, surfaceFormats.data());

		// Pick surface format
		for (const auto& surfaceFormat : surfaceFormats)
		{
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected desired surface format");
				return surfaceFormat;
			}
		}

		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected default surface format");
		return surfaceFormats.at(0);
	}

	static VkPresentModeKHR PickPresentMode(const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface)
	{
		// Query available present modes
		uint32 presentModeCount{ 0 };
		vkGetPhysicalDeviceSurfacePresentModesKHR(_gpu, _surface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_gpu, _surface, &presentModeCount, presentModes.data());

		// Pick present mode
		for (const auto& presentMode : presentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected desired presentation mode");
				return presentMode;
			}
		}

		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected default presentation mode");

		// If mailbox present mode is not available, choose FIFO
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	static VkExtent2D PickExtent(const VkSurfaceCapabilitiesKHR& _surfaceCapabilities, const uint32 _w, const uint32 _h) noexcept
	{
		// If the surface extent is undefined, set it to the window size (_w, _h)
		if (_surfaceCapabilities.currentExtent.width == UINT32_MAX)
		{
			VkExtent2D extent{ _w, _h };
			extent.width = std::max(_surfaceCapabilities.minImageExtent.width, std::min(_surfaceCapabilities.maxImageExtent.width, extent.width));
			extent.height = std::max(_surfaceCapabilities.minImageExtent.height, std::min(_surfaceCapabilities.maxImageExtent.height, extent.height));
			return extent;
		}

		// Otherwise, use the surface extent
		return _surfaceCapabilities.currentExtent;
	}

	static uint32 PickImageCount(const VkSurfaceCapabilitiesKHR& _surfaceCapabilities)
	{
		const uint32 imageCount{ _surfaceCapabilities.minImageCount + 1 };

		if (_surfaceCapabilities.maxImageCount > 0 && imageCount > _surfaceCapabilities.maxImageCount)
		{
			SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Picked image count of %d", _surfaceCapabilities.maxImageCount);
			return _surfaceCapabilities.maxImageCount;
		}

		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Picked image count of %d", imageCount);
		return imageCount;
	}

	VulkanSwapchain::VulkanSwapchain(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface, const uint32 _w, const uint32 _h) :
		m_Swapchain{ VK_NULL_HANDLE },
		m_Device{ _logicalDevice },
		m_GPU{ _gpu },
		m_Surface{ _surface },
		m_SwapchainImages{},
		m_SwapchainImageViews{},
		m_Format{ VK_FORMAT_UNDEFINED }
	{
		CreateSwapchain(_w, _h);
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		for (const auto& imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(m_Device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
		m_Swapchain = VK_NULL_HANDLE;
	}

	void VulkanSwapchain::RecreateSwapchain(const uint32 _w, const uint32 _h)
	{
		for (const auto& imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(m_Device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
		m_Swapchain = VK_NULL_HANDLE;

		CreateSwapchain(_w, _h);
	}

	void VulkanSwapchain::CreateSwapchain(const uint32 _w, const uint32 _h)
	{
		SE_LOG(LogCategory::Trace, "[SWAPCHAIN]: Creating Vulkan Swapchain");

		// Query surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_GPU, m_Surface, &surfaceCapabilities);

		// Pick swapchain surface format and color space
		const VkSurfaceFormatKHR surfaceFormat{ PickSurfaceFormat(m_GPU, m_Surface) };
		m_Format = surfaceFormat.format;

		// Pick swapchain present mode
		const VkPresentModeKHR presentMode{ PickPresentMode(m_GPU, m_Surface) };

		// Pick swapchain extent
		const VkExtent2D extent{ PickExtent(surfaceCapabilities, _w, _h) };
		m_Width = extent.width;
		m_Height = extent.height;

		// Determine number of images in the swapchain
		uint32 imageCount{ PickImageCount(surfaceCapabilities) };

		// Create swapchain
		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = m_Surface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = extent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(m_Device, &swapchainCreateInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create Vulkan swapchain\n");
		}

		// Retrieve swapchain images
		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, m_SwapchainImages.data());

		// Create image views for the swapchain images
		m_SwapchainImageViews.resize(imageCount);

		for (uint32 i = 0; i < imageCount; ++i)
		{
			VulkanUtils::CreateImageView(m_Device, m_SwapchainImages.at(i), surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, m_SwapchainImageViews.at(i));
		}

		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Created Vulkan Swapchain");
	}
} // End of namespace