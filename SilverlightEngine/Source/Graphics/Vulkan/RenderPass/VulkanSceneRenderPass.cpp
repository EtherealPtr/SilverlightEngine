#include "VulkanSceneRenderPass.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>
#include <array>

namespace Silverlight
{
	VulkanSceneRenderPass::VulkanSceneRenderPass(const VkDevice& _device, const VkFormat _colorFormat, const VkFormat _depthFormat) : 
		VulkanRenderPassBase{ _device },
		m_ColorFormat{ _colorFormat },
		m_DepthFormat{ _depthFormat }
	{
		CreateRenderPass();
	}

	void VulkanSceneRenderPass::CreateRenderPass()
	{
		SE_LOG(LogCategory::Trace, "[RENDER PASS]: Creating scene render pass");

		std::array<VkAttachmentDescription, 2> attachments{};
		std::array<VkAttachmentReference, 2> attachmentReferences{};

		// Define a color attachment for the color output
		attachments.at(0).format = static_cast<VkFormat>(m_ColorFormat);
		attachments.at(0).samples = VK_SAMPLE_COUNT_1_BIT;
		attachments.at(0).loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments.at(0).storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments.at(0).stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments.at(0).stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments.at(0).initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments.at(0).finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Define a depth attachment for the depth buffer
		attachments.at(1).format = static_cast<VkFormat>(m_DepthFormat);
		attachments.at(1).samples = VK_SAMPLE_COUNT_1_BIT;
		attachments.at(1).loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments.at(1).storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments.at(1).stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments.at(1).stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments.at(1).initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments.at(1).finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Reference to the color attachment
		attachmentReferences.at(0).attachment = 0;
		attachmentReferences.at(0).layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Reference to the depth attachment
		attachmentReferences.at(1).attachment = 1;
		attachmentReferences.at(1).layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// Define the subpass
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &attachmentReferences.at(0);
		subpass.pDepthStencilAttachment = &attachmentReferences.at(1);

		// Define the subpass dependency
		VkSubpassDependency externalToSubpass0Dependency{};
		externalToSubpass0Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		externalToSubpass0Dependency.dstSubpass = 0;
		externalToSubpass0Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		externalToSubpass0Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		externalToSubpass0Dependency.srcAccessMask = 0;
		externalToSubpass0Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		// Define the render pass
		VkRenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32>(attachments.size());
		renderPassCreateInfo.pAttachments = attachments.data();
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpass;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &externalToSubpass0Dependency;

		if (vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create the scene render pass");
		}

		SE_LOG(LogCategory::Info, "[RENDER PASS]: Created scene render pass");
	}
} // End of namespace