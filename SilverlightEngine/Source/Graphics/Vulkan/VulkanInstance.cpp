#include "VulkanInstance.h"
#include "VulkanUtils.h"
#include "VulkanValidation.h"
#include "Foundation/Logging/Logger.h"
#include <GLFW/glfw3.h>

#ifdef NDEBUG
constexpr bool g_EnableValidationLayers{ false };
#else
constexpr bool g_EnableValidationLayers{ true };
#endif

namespace Silverlight
{
	VulkanInstance::VulkanInstance() :
		m_Instance{ VK_NULL_HANDLE },
		m_DebugMessenger{ VK_NULL_HANDLE }
	{
		SE_LOG(LogCategory::Trace, "[INSTANCE]: Creating Vulkan instance");

		uint32 instanceVersion{ 0 };
		vkEnumerateInstanceVersion(&instanceVersion);

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "None";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 2, 0);
		appInfo.pEngineName = "None";
		appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 2, 0);
		appInfo.apiVersion = instanceVersion >= VK_API_VERSION_1_4 ? VK_API_VERSION_1_4 : VK_API_VERSION_1_3;

		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;

		// Query the required extensions for GLFW
		unsigned int glfwExtensionCount{ 0 };
		const char** glfwExtensions{ glfwGetRequiredInstanceExtensions(&glfwExtensionCount) };

		// List the required instance extensions
		std::vector<const char*> requiredInstanceExtensions{};
		requiredInstanceExtensions.reserve((size_t)glfwExtensionCount + 1);

		for (size_t i = 0; i < glfwExtensionCount; ++i)
		{
			requiredInstanceExtensions.emplace_back(glfwExtensions[i]);
		}

		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;

		std::vector<const char*> validationLayers{};

		if (g_EnableValidationLayers)
		{
			requiredInstanceExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);;

			// Enable validation layers
			validationLayers.reserve(1);
			validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");

			VulkanUtils::CheckInstanceLayerSupport(validationLayers);
			instanceCreateInfo.enabledLayerCount = (uint32)(validationLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		}

		// Enable instance extensions
		VulkanUtils::CheckInstanceExtSupport(requiredInstanceExtensions);
		instanceCreateInfo.enabledExtensionCount = (uint32)(requiredInstanceExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions.data();

		if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create Vulkan instance\n");
		}

		SetupDebugCallback();

		SE_LOG(LogCategory::Info, "[INSTANCE]: Created Vulkan instance");
	}

	VulkanInstance::~VulkanInstance()
	{
		if (g_EnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
			m_DebugMessenger = VK_NULL_HANDLE;
		}

		vkDestroyInstance(m_Instance, nullptr);
		m_Instance = VK_NULL_HANDLE;
	}

	void VulkanInstance::SetupDebugCallback()
	{
		if (!g_EnableValidationLayers)
		{
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = nullptr;

		if (CreateDebugUtilsMessengerEXT(m_Instance, &debugCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
		{
			SE_LOG(LogCategory::Warning, "[INSTANCE]: Failed to create vulkan debug messenger");
		}
	}
} // End of namespace