#include "VulkanSkyboxPipeline.h"
#include "Graphics/Vulkan/VulkanRenderContext.h"
#include "Graphics/Vulkan/VulkanUtils.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/Logging/Logger.h"
#include "Graphics/Vertex.h"
#include "Graphics/DataStructs.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanSkyboxPipeline::VulkanSkyboxPipeline(const VulkanRenderContext& _renderContext, const VkDescriptorSetLayout& _descriptorSetLayout, const VkRenderPass& _renderPass) :
		VulkanPipelineBase{ _renderContext, _descriptorSetLayout, _renderPass },
		m_Width{ _renderContext.GetSwapchain().GetWidth() },
		m_Height{ _renderContext.GetSwapchain().GetHeight() }
	{
		CreatePipeline();
	}

	void VulkanSkyboxPipeline::CreatePipeline()
	{
		SE_LOG(LogCategory::Info, "[SKYBOX PIPELINE]: Creating skybox graphics pipeline");

		const auto vertShaderBinary{ g_ResourceManager.ReadBinaryFile("Shaders/skybox.vert.spv") };
		const auto fragShaderBinary{ g_ResourceManager.ReadBinaryFile("Shaders/skybox.frag.spv") };

		VkShaderModule vertexShaderModule{ VulkanUtils::CreateShaderModule(m_Device, vertShaderBinary) };
		VkShaderModule fragmentShaderModule{ VulkanUtils::CreateShaderModule(m_Device, fragShaderBinary) };

		// Shader creation stage
		VkPipelineShaderStageCreateInfo vertexShaderCreateInfo{};
		vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderCreateInfo.module = vertexShaderModule;
		vertexShaderCreateInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo{};
		fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderCreateInfo.module = fragmentShaderModule;
		fragmentShaderCreateInfo.pName = "main";

		const VkPipelineShaderStageCreateInfo shaderStageCreateInfos[]{ vertexShaderCreateInfo, fragmentShaderCreateInfo };

		// Vertex input stage
		VkVertexInputBindingDescription inputBindingDescription{};
		inputBindingDescription.binding = 0;
		inputBindingDescription.stride = sizeof(Vertex);
		inputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::array<VkVertexInputAttributeDescription, 1> inputAttributeDescriptions{};
		inputAttributeDescriptions.at(0).binding = 0;
		inputAttributeDescriptions.at(0).location = 0;
		inputAttributeDescriptions.at(0).format = VK_FORMAT_R32G32B32_SFLOAT;
		inputAttributeDescriptions.at(0).offset = 0;

		VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
		vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
		vertexInputCreateInfo.pVertexBindingDescriptions = &inputBindingDescription;
		vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32>(inputAttributeDescriptions.size());
		vertexInputCreateInfo.pVertexAttributeDescriptions = inputAttributeDescriptions.data();

		// Dynamic states stage
		std::vector<VkDynamicState> dynamicState =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32>(dynamicState.size());
		dynamicStateCreateInfo.pDynamicStates = dynamicState.data();

		// Input assembly stage
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{};
		inputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

		// Viewports and scissors stage
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_Width);
		viewport.height = static_cast<float>(m_Height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ m_Width, m_Height });

		VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
		viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateCreateInfo.viewportCount = 1;
		viewportStateCreateInfo.pViewports = &viewport;
		viewportStateCreateInfo.scissorCount = 1;
		viewportStateCreateInfo.pScissors = &scissor;

		// Rasterizer stage
		VkPipelineRasterizationStateCreateInfo rasterizerStateCreateInfo{};
		rasterizerStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerStateCreateInfo.depthClampEnable = VK_FALSE;
		rasterizerStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizerStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizerStateCreateInfo.lineWidth = 1.0f;
		rasterizerStateCreateInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
		rasterizerStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizerStateCreateInfo.depthBiasEnable = VK_FALSE;
		rasterizerStateCreateInfo.depthBiasConstantFactor = 0.0f;
		rasterizerStateCreateInfo.depthBiasClamp = 0.0f;
		rasterizerStateCreateInfo.depthBiasSlopeFactor = 0.0f;

		// Multisampling stage
		VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo{};
		multisamplingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisamplingStateCreateInfo.sampleShadingEnable = VK_FALSE;
		multisamplingStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisamplingStateCreateInfo.minSampleShading = 1.0f;
		multisamplingStateCreateInfo.pSampleMask = nullptr;
		multisamplingStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
		multisamplingStateCreateInfo.alphaToOneEnable = VK_FALSE;

		// Color blending stage
		VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};
		colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachmentState.blendEnable = VK_FALSE;
		colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
		colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
		colorBlendStateCreateInfo.attachmentCount = 1;
		colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;

		// Depth stencil stage
		VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo{};
		depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilCreateInfo.depthTestEnable = VK_TRUE;
		depthStencilCreateInfo.depthWriteEnable = VK_FALSE;
		depthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilCreateInfo.minDepthBounds = 0.0f;
		depthStencilCreateInfo.maxDepthBounds = 1.0f;
		depthStencilCreateInfo.stencilTestEnable = VK_FALSE;
		depthStencilCreateInfo.front = {};
		depthStencilCreateInfo.back = {};

		// Pipeline layout stage
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = VK_NULL_HANDLE;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;

		if (vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a pipeline layout");
		}

		VkGraphicsPipelineCreateInfo skyboxPipelineCreateInfo{};
		skyboxPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		skyboxPipelineCreateInfo.stageCount = 2;
		skyboxPipelineCreateInfo.pStages = shaderStageCreateInfos;
		skyboxPipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
		skyboxPipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;
		skyboxPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
		skyboxPipelineCreateInfo.pRasterizationState = &rasterizerStateCreateInfo;
		skyboxPipelineCreateInfo.pMultisampleState = &multisamplingStateCreateInfo;
		skyboxPipelineCreateInfo.pDepthStencilState = &depthStencilCreateInfo;
		skyboxPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
		skyboxPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
		skyboxPipelineCreateInfo.layout = m_PipelineLayout;
		skyboxPipelineCreateInfo.renderPass = m_RenderPass;
		skyboxPipelineCreateInfo.subpass = 0;
		skyboxPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		skyboxPipelineCreateInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &skyboxPipelineCreateInfo, nullptr, &m_Pipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a skybox graphics pipeline");
		}

		vkDestroyShaderModule(m_Device, vertexShaderModule, nullptr);
		vkDestroyShaderModule(m_Device, fragmentShaderModule, nullptr);

		SE_LOG(LogCategory::Info, "[SKYBOX PIPELINE]: Created skybox graphics pipeline");
	}
} // End of namespace