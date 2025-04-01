#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkImageView_T* VkImageView;
typedef struct VkFramebuffer_T* VkFramebuffer;

namespace Silverlight
{
	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkImageView& _colorImageView, const VkImageView& _depthImageView, const uint32 _w, const uint32 _h);
		~VulkanFramebuffer();

		void RecreateFramebuffer(const uint32 _w, const uint32 _h, const VkImageView& _coloredImageView, const VkImageView& _depthImageView);
		const VkFramebuffer& Get() const noexcept { return m_Framebuffer; }

	private:
		void CreateFramebuffer(uint32 _w, uint32 _h, const VkImageView& _colorImageView, const VkImageView& _depthImageView);
		void CleanUp() noexcept;

	private:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;
		VkFramebuffer m_Framebuffer;
	};
} // End of namespace