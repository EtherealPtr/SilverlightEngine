#include "VulkanSwapchain.h"
#include "VulkanUtils.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>
#include <algorithm>

namespace Silverlight
{
	static VkSurfaceFormatKHR PickSurfaceFormat(const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface)
	{
		uint32 formatCount{ 0 };
		vkGetPhysicalDeviceSurfaceFormatsKHR(_gpu, _surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_gpu, _surface, &formatCount, surfaceFormats.data());

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
		uint32 presentModeCount{ 0 };
		vkGetPhysicalDeviceSurfacePresentModesKHR(_gpu, _surface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_gpu, _surface, &presentModeCount, presentModes.data());

		for (const auto& presentMode : presentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected desired presentation mode");
				return presentMode;
			}
		}

		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected default presentation mode");
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

		return _surfaceCapabilities.currentExtent;
	}

	static uint32 PickImageCount(const VkSurfaceCapabilitiesKHR& _surfaceCapabilities)
	{
		const uint32 imageCount{ _surfaceCapabilities.minImageCount + 1 };

		if (_surfaceCapabilities.maxImageCount > 0 && imageCount > _surfaceCapabilities.maxImageCount)
		{
			SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Picked image count of {}", _surfaceCapabilities.maxImageCount);
			return _surfaceCapabilities.maxImageCount;
		}

		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Picked image count of {}", imageCount);
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
		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Creating Vulkan Swapchain");

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_GPU, m_Surface, &surfaceCapabilities);

		const VkSurfaceFormatKHR surfaceFormat{ PickSurfaceFormat(m_GPU, m_Surface) };
		m_Format = surfaceFormat.format;

		const VkPresentModeKHR presentMode{ PickPresentMode(m_GPU, m_Surface) };

		const VkExtent2D extent{ PickExtent(surfaceCapabilities, _w, _h) };
		m_Width = extent.width;
		m_Height = extent.height;

		uint32 imageCount{ PickImageCount(surfaceCapabilities) };

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

		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &imageCount, m_SwapchainImages.data());

		m_SwapchainImageViews.resize(imageCount);

		for (uint32 i = 0; i < imageCount; ++i)
		{
			VulkanUtils::CreateImageView(m_Device, m_SwapchainImages.at(i), surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT, m_SwapchainImageViews.at(i));
		}

		SE_LOG(LogCategory::Info, "[SWAPCHAIN]: Created Vulkan Swapchain");
	}
} // End of namespace