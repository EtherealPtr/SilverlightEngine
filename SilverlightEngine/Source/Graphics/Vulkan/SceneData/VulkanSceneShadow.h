#pragma once

#include "Graphics/Vulkan/VulkanDepthBuffer.h"
#include "Graphics/Vulkan/RenderPass/VulkanShadowRenderPass.h"
#include "Graphics/Vulkan/VulkanFramebuffer.h"
#include "Graphics/Vulkan/Pipeline/VulkanShadowPipeline.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"
#include "Graphics/Vulkan/VulkanDescriptorSet.h"
#include "Graphics/Vulkan/VulkanDescriptorInfo.h"
#include "Graphics/GLMConfig.h"
#include <vector>

namespace Silverlight
{
	class VulkanRenderContext;
	class VulkanDescriptorSetLayout;
	class LightSystem;

	class VulkanSceneShadow
	{
	public:
		VulkanSceneShadow(const VulkanRenderContext& _renderContext, const VulkanDescriptorSetLayout& _setLayout, const VkDescriptorPool& _descriptorPool);

		VulkanSceneShadow(const VulkanSceneShadow&) = delete;
		VulkanSceneShadow& operator=(const VulkanSceneShadow&) = delete;
		VulkanSceneShadow(VulkanSceneShadow&&) = delete;
		VulkanSceneShadow& operator=(VulkanSceneShadow&&) = delete;

		uint32 GetShadowWidth() const noexcept { return m_Width; }
		uint32 GetShadowHeight() const noexcept { return m_Height; }
		void UpdateLightSpaceUniformBuffer(const LightSystem& _lightSystem, const uint32 _imgIndex);
		void RecreateResources(const uint32 _flags);
		const VulkanDepthBuffer& GetDepthBuffer() const noexcept { return m_DepthBuffer; }
		const VulkanShadowRenderPass& GetRenderPass() const noexcept { return m_RenderPass; }
		const VulkanFramebuffer& GetFramebuffer() const noexcept { return m_Framebuffer; }
		const VulkanShadowPipeline& GetPipeline() const noexcept { return m_Pipeline; }
		const std::vector<VulkanDescriptorSet>& GetDescriptorSets() noexcept { return m_DescriptorSets; }
		const VulkanBufferDescriptorInfo& GetBufferDescriptorInfo() const noexcept { return m_BufferDescriptorInfo; }

	private:
		uint32 m_Width;
		uint32 m_Height;
		VulkanDepthBuffer m_DepthBuffer;
		VulkanShadowRenderPass m_RenderPass;
		VulkanShadowPipeline m_Pipeline;
		VulkanFramebuffer m_Framebuffer;
		VulkanBufferDescriptorInfo m_BufferDescriptorInfo;
		glm::mat4 m_LightSpaceMatrix;
		std::vector<VulkanUniformBuffer> m_LightSpaceUniformBuffers;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
	};
} // End of namespace