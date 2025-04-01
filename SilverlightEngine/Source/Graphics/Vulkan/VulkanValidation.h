#pragma once

#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT _msgSeverity,
		VkDebugUtilsMessageTypeFlagsEXT _msgType,
		const VkDebugUtilsMessengerCallbackDataEXT* _pCallbackData,
		void* _pUserData)
	{
		if (_msgSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			printf("%s\n", _pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance _instance,
		const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo,
		const VkAllocationCallbacks* _pAllocator,
		VkDebugUtilsMessengerEXT* _pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr)
		{
			return func(_instance, _pCreateInfo, _pAllocator, _pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(
		VkInstance _instance,
		VkDebugUtilsMessengerEXT _debugMessenger,
		const VkAllocationCallbacks* _pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");

		if (func != nullptr)
		{
			func(_instance, _debugMessenger, _pAllocator);
		}
	}
} // End of namespace