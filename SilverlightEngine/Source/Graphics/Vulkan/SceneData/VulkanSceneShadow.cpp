#include "VulkanSceneShadow.h"
#include "Graphics/Vulkan/VulkanRenderContext.h"
#include "Graphics/Systems/LightSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanSceneShadow::VulkanSceneShadow(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags) :
		m_Width{ 1024 },
		m_Height{ 1024 },
		m_DepthBuffer{ _renderContext.GetDevice(), m_Width, m_Height, _flags },
		m_RenderPass{ _renderContext.GetDevice().GetLogicalDevice(), m_DepthBuffer.GetFormat() },
		m_FramebufferShadow{ _renderContext.GetDevice().GetLogicalDevice(), m_RenderPass.Get(), VK_NULL_HANDLE, m_DepthBuffer.GetImageView(), m_Width, m_Height },
		m_DescriptorSetLayout{ _renderContext.GetDevice().GetLogicalDevice() },
		m_Pipeline{ _renderContext, m_DescriptorSetLayout.Get(), m_RenderPass.Get(), m_Width, m_Height },
		m_LightSpaceMatrix{ glm::identity<glm::mat4>() },
		m_DescriptorSetWriters{ 1, { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER } }
	{
		const size_t numOfSwapChainImages{ _renderContext.GetSwapchain().GetImageViews().size() };

		m_LightSpaceUniformBuffers.reserve(numOfSwapChainImages);
		m_DescriptorSets.reserve(numOfSwapChainImages);

		for (size_t i = 0; i < numOfSwapChainImages; ++i)
		{
			m_DescriptorSets.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), _descriptorPool, m_DescriptorSetLayout.Get());
			m_LightSpaceUniformBuffers.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetDevice().GetPhysicalDevice(), sizeof(glm::mat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		}
	}

	void VulkanSceneShadow::UpdateLightSpaceUniformBuffers(const LightSystem& _lightSystem, const uint32 _imgIndex)
	{
		auto directionalLightComponent{ _lightSystem.GetDirectionalLight() };
		if (!directionalLightComponent.has_value()) return;

		const glm::vec3 lightDir{ glm::normalize(directionalLightComponent->get()->GetLightData().m_Direction) };
		const glm::vec3 lightPos{ -lightDir * 10.0f };
		const glm::mat4 lightViewMatrix{ glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) };
		const glm::mat4 lightProjMatrix{ glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 50.0f) };
		m_LightSpaceMatrix = lightProjMatrix * lightViewMatrix;

		m_LightSpaceUniformBuffers.at(_imgIndex).CopyData(&m_LightSpaceMatrix);
		m_DescriptorSetWriters.at(0).SetBuffer(m_LightSpaceUniformBuffers.at(_imgIndex).GetBuffer(), m_LightSpaceUniformBuffers.at(_imgIndex).GetBufferSize());
		m_DescriptorSets.at(_imgIndex).UpdateDescriptorSet(m_DescriptorSetWriters);
	}

	void VulkanSceneShadow::RecreateResources(const uint32 _flags)
	{
		m_DepthBuffer.RecreateDepthBuffer(m_Width, m_Height, _flags);
		m_FramebufferShadow.RecreateFramebuffer(m_Width, m_Height, VK_NULL_HANDLE, m_DepthBuffer.GetImageView());
	}
} // End of namespace