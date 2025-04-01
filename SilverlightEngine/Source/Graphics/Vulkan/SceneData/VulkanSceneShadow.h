#pragma once

#include "Graphics/Vulkan/VulkanDepthBuffer.h"
#include "Graphics/Vulkan/RenderPass/VulkanShadowRenderPass.h"
#include "Graphics/Vulkan/VulkanFramebuffer.h"
#include "Graphics/Vulkan/Layout/VulkanShadowSceneDiscriptorSetLayout.h"
#include "Graphics/Vulkan/Pipeline/VulkanShadowPipeline.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"
#include "Graphics/Vulkan/VulkanDescriptorSet.h"
#include "Graphics/Vulkan/VulkanDescriptorSetWriters.h"
#include <glm/glm.hpp>
#include <vector>

namespace Silverlight
{
	class VulkanRenderContext;
	class LightSystem;

	class VulkanSceneShadow
	{
	public:
		VulkanSceneShadow(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags);

		VulkanSceneShadow(const VulkanSceneShadow&) = delete;
		VulkanSceneShadow& operator=(const VulkanSceneShadow&) = delete;
		VulkanSceneShadow(VulkanSceneShadow&&) = delete;
		VulkanSceneShadow& operator=(VulkanSceneShadow&&) = delete;

		uint32 GetShadowWidth() const noexcept { return m_Width; }
		uint32 GetShadowHeight() const noexcept { return m_Height; }
		void UpdateLightSpaceUniformBuffers(const LightSystem& _lightSystem, const uint32 _imgIndex);
		void RecreateResources(const uint32 _flags);
		const std::vector<VulkanUniformBuffer>& GetLightSpaceUniformBuffers() const noexcept { return m_LightSpaceUniformBuffers; }
		const VulkanDepthBuffer& GetDepthBuffer() const noexcept { return m_DepthBuffer; }
		const VkRenderPass& GetRenderPass() const noexcept { return m_RenderPass.Get(); }
		const VulkanFramebuffer& GetFramebuffer() const noexcept { return m_FramebufferShadow; }
		const VulkanShadowSceneDiscriptorSetLayout& GetDescriptorSetLayout() const noexcept { return m_DescriptorSetLayout; }
		const VulkanShadowPipeline& GetPipeline() const noexcept { return m_Pipeline; }
		const std::vector<VulkanDescriptorSet>& GetDescriptorSet() noexcept { return m_DescriptorSets; }
		const glm::mat4& GetLightSpaceMatrix() const noexcept { return m_LightSpaceMatrix; }

	private:
		uint32 m_Width;
		uint32 m_Height;
		VulkanDepthBuffer m_DepthBuffer;
		VulkanShadowRenderPass m_RenderPass;
		VulkanFramebuffer m_FramebufferShadow;
		VulkanShadowSceneDiscriptorSetLayout m_DescriptorSetLayout;
		VulkanShadowPipeline m_Pipeline;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
		std::vector<VulkanDescriptorSetBufferWriter> m_DescriptorSetWriters;
		std::vector<VulkanUniformBuffer> m_LightSpaceUniformBuffers;
		glm::mat4 m_LightSpaceMatrix;
	};
} // End of namespace