#pragma once

#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

namespace Silverlight
{
    class Window;

	class VulkanRenderContext
	{
    public:
        VulkanRenderContext(const Window& _window);

        VulkanRenderContext(const VulkanRenderContext&) = delete;
        VulkanRenderContext& operator=(const VulkanRenderContext&) = delete;
        VulkanRenderContext(VulkanRenderContext&&) = delete;
        VulkanRenderContext& operator=(VulkanRenderContext&&) = delete;

        const Window& GetWindow() const noexcept { return m_Window; }
        void RecreateSwapchain(const uint32 _w, const uint32 _h);
        const VulkanDevice& GetDevice() const noexcept { return m_Device; }
        const VulkanSwapchain& GetSwapchain() const noexcept { return m_Swapchain; }

    private:
        const Window& m_Window;
        VulkanInstance m_Instance;
        VulkanSurface m_Surface;
        VulkanDevice m_Device;
        VulkanSwapchain m_Swapchain;
	};
} // End of namespace