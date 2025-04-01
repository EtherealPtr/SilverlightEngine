#pragma once

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef enum VkFormat VkFormat;

namespace Silverlight
{
	class VulkanRenderPassBase
	{
	public:
		VulkanRenderPassBase(const VkDevice& _device) noexcept;
		virtual ~VulkanRenderPassBase() noexcept { CleanUp(); }

		VulkanRenderPassBase(const VulkanRenderPassBase&) = delete;
		VulkanRenderPassBase& operator=(const VulkanRenderPassBase&) = delete;
		VulkanRenderPassBase(VulkanRenderPassBase&&) = delete;
		VulkanRenderPassBase& operator=(VulkanRenderPassBase&&) = delete;

		void CleanUp() noexcept;
		const VkRenderPass& Get() const noexcept { return m_RenderPass; }

	protected:
		virtual void CreateRenderPass() = 0;

	protected:
		VkDevice m_Device;
		VkRenderPass m_RenderPass;
	};
} // End of namespace