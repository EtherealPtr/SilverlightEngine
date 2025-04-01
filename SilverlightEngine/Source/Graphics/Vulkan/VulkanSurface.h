#pragma once

typedef struct VkInstance_T* VkInstance;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
struct GLFWwindow;

namespace Silverlight
{
	class VulkanSurface
	{
	public:
		VulkanSurface(GLFWwindow* _window, const VkInstance& _instance);
		~VulkanSurface();

		VulkanSurface(const VulkanSurface&) = delete;
		VulkanSurface& operator=(const VulkanSurface&) = delete;
		VulkanSurface(VulkanSurface&&) = delete;
		VulkanSurface& operator=(VulkanSurface&&) = delete;

		const VkSurfaceKHR& Get() const noexcept { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface;
		VkInstance m_VkInstance;
	};
} // End of namespace