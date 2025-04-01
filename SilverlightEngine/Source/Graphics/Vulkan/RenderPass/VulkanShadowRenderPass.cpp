#include "VulkanShadowRenderPass.h"
#include "Foundation/Logging/Logger.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanShadowRenderPass::VulkanShadowRenderPass(const VkDevice& _device, const VkFormat _depthFormat) : 
		VulkanRenderPassBase{ _device },
		m_DepthFormat{ _depthFormat }
	{
		CreateRenderPass();
	}

	void VulkanShadowRenderPass::CreateRenderPass()
	{
		SE_LOG(LogCategory::Trace, "[RENDER PASS]: Creating depth-only render pass");
		
		VkAttachmentDescription attachment{};
		attachment.format = static_cast<VkFormat>(m_DepthFormat);
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		
		VkAttachmentReference attachmentReference{};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 0;
		subpass.pDepthStencilAttachment = &attachmentReference;
		
		std::array<VkSubpassDependency, 2> dependencies{};
		
		dependencies.at(0).srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies.at(0).dstSubpass = 0;
		dependencies.at(0).srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies.at(0).dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependencies.at(0).srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies.at(0).dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies.at(0).dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
		
		dependencies.at(1).srcSubpass = 0;
		dependencies.at(1).dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies.at(1).srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies.at(1).dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies.at(1).srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies.at(1).dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies.at(1).dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
		
		VkRenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = &attachment;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpass;
		renderPassCreateInfo.dependencyCount = static_cast<uint32>(dependencies.size());
		renderPassCreateInfo.pDependencies = dependencies.data();
		
		if (vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a depth-only render pass");
		}
		
		SE_LOG(LogCategory::Info, "[RENDER PASS]: Created depth-only render pass");
	}
} // End of namespace