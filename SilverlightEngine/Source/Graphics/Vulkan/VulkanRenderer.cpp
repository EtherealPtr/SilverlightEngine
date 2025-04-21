#include "VulkanRenderer.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Paths/PathManager.h"
#include "Graphics/Window.h"
#include "Graphics/Components/Mesh/PrimitiveMeshComponent.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanRenderer::VulkanRenderer(const Window& _window) :
		m_RenderContext{ _window },
		m_DescriptorPool{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetSwapchain().GetImageViews().size() * 3 },
		m_Scene{ m_RenderContext, m_DescriptorPool.Get(), 0 },
		m_CommandPool{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetQueueIndices().m_GraphicsQueueFamilyIndex },
		m_CommandBuffers{ m_RenderContext.GetDevice().GetLogicalDevice(), m_CommandPool.Get(), static_cast<uint16>(m_RenderContext.GetSwapchain().GetImageViews().size()) },
		m_Semaphore{ m_RenderContext.GetDevice().GetLogicalDevice(), static_cast<uint16>(m_RenderContext.GetSwapchain().GetImageViews().size()) },
		m_InFlightFences{ m_RenderContext.GetDevice().GetLogicalDevice(), static_cast<uint16>(m_RenderContext.GetSwapchain().GetImageViews().size()) },
		m_TextureSampler{ m_RenderContext.GetDevice(), VK_SAMPLER_ADDRESS_MODE_REPEAT },
		m_CubemapSampler{ m_RenderContext.GetDevice(), VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE },
		m_TextureManager{ m_RenderContext.GetDevice(), m_CommandPool.Get() },
		m_VertexBufferManager{ m_RenderContext.GetDevice(), m_CommandPool.Get(), m_RenderContext.GetDevice().GetGraphicsQueue() },
		m_Camera{ 80.0f, static_cast<float>(_window.GetFramebufferWidth()) / _window.GetFramebufferHeight(), 0.1f, 100.0f, _window.GetWindow() },
		m_MeshSystem{ m_VertexBufferManager },
		m_LightSystem{},
		m_TransformationSystem{},
		m_CurrentFrameIndex{ 0 }
	{
		LoadDefaultCubemap();
		InitializeGraphicsComponents();

		m_CastShadows = m_LightSystem.GetDirectionalLight().has_value();

		if (m_CastShadows)
		{
			m_ShadowScene.emplace(m_RenderContext, m_Scene.GetDescriptorSetLayout(), m_DescriptorPool.Get());
			m_Scene.SetSceneShadowMap(m_ShadowScene.value().GetDepthBuffer().GetImageView(), m_TextureSampler.Get());
		}
		else
		{
			VkImageView dummyShadowMap{ m_TextureManager.GetDummyDepthTexture() };
			m_Scene.SetSceneShadowMap(dummyShadowMap, m_TextureSampler.Get());
		}

		m_Scene.UpdateMaterialUniformBuffers(m_MeshSystem);
		m_Scene.SetSceneTextures(m_TextureManager.GetTextureImageViews(), m_TextureSampler.Get());
		m_Scene.SetSceneCubemap(m_TextureManager.GetCubemapTexture(), m_CubemapSampler.Get());

		SE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan renderer initialized");
	}

	VulkanRenderer::~VulkanRenderer()
	{
		vkDeviceWaitIdle(m_RenderContext.GetDevice().GetLogicalDevice());
		SE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan renderer shutting down");
	}

	void VulkanRenderer::LoadDefaultCubemap()
	{
		const std::string path{ PathManager::GetEngineAssetsPath() };

		std::array<std::string, 6> skyboxFaces
		{
			path + "Skybox/right.jpg",
			path + "Skybox/left.jpg",
			path + "Skybox/top.jpg",
			path + "Skybox/bottom.jpg",
			path + "Skybox/front.jpg",
			path + "Skybox/back.jpg"
		};

		m_TextureManager.CreateCubemap(skyboxFaces);
	}

	void VulkanRenderer::InitializeGraphicsComponents()
	{
		m_CubemapMeshId = m_MeshSystem.CreatePrimitiveMesh(PrimitiveShapeEnum::CubeShape);

		for (const auto& entity : EntityManager::GetAllEntities())
		{
			m_MeshSystem.ProcessComponents(entity);
			m_LightSystem.ProcessComponents(entity);
			m_TransformationSystem.ProcessComponents(entity);
		}
	}

	void VulkanRenderer::DrawFrame(const double _deltaTime)
	{
		uint32 imgIndex{ 0 };

		m_InFlightFences.Wait(m_CurrentFrameIndex);

		const VkDevice logicalDevice{ m_RenderContext.GetDevice().GetLogicalDevice() };
		const VkSwapchainKHR swapchain{ m_RenderContext.GetSwapchain().Get() };
		const VkResult result{ vkAcquireNextImageKHR(logicalDevice, swapchain, UINT64_MAX, m_Semaphore.Get().at(m_CurrentFrameIndex).first, VK_NULL_HANDLE, &imgIndex) };

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			vkDeviceWaitIdle(logicalDevice);

			const uint32 newWidth{ (uint32)m_RenderContext.GetWindow().GetFramebufferWidth() };
			const uint32 newHeight{ (uint32)m_RenderContext.GetWindow().GetFramebufferHeight() };

			if (newWidth == 0 || newHeight == 0)
			{
				return;
			}

			m_RenderContext.RecreateSwapchain(newWidth, newHeight);
			m_Scene.RecreateResources(newWidth, newHeight, 0);

			if (m_CastShadows)
			{
				m_ShadowScene.value().RecreateResources(VK_IMAGE_USAGE_SAMPLED_BIT);
				m_Scene.SetSceneShadowMap(m_ShadowScene.value().GetDepthBuffer().GetImageView(), m_TextureSampler.Get());
			}

			vkResetCommandPool(logicalDevice, m_CommandPool.Get(), 0);
			return;
		}

		m_InFlightFences.Reset(m_CurrentFrameIndex);

		m_Camera.ProcessInput(_deltaTime);

		VkSemaphore waitSemaphore{ m_Semaphore.Get().at(m_CurrentFrameIndex).first };
		VkSemaphore signalSemaphore{ m_Semaphore.Get().at(m_CurrentFrameIndex).second };

		VkCommandBuffer cmdBuffer{ m_CommandBuffers.Get().at(imgIndex) };

		vkResetCommandBuffer(cmdBuffer, 0);
		RecordRenderCommands(imgIndex);

		m_CommandBuffers.Submit
		(
			imgIndex,
			m_RenderContext.GetDevice().GetGraphicsQueue(),
			waitSemaphore,
			signalSemaphore,
			m_InFlightFences.Get().at(m_CurrentFrameIndex),
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &signalSemaphore;

		VkSwapchainKHR swapchains[] = { swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imgIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(m_RenderContext.GetDevice().GetPresentationQueue(), &presentInfo);
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_RenderContext.GetSwapchain().GetImageViews().size();
	}

	void VulkanRenderer::RecordRenderCommands(const uint32 _imgIndex)
	{
		const VkCommandBuffer cmdBuffer{ m_CommandBuffers.Get().at(_imgIndex) };
		m_CommandBuffers.Begin(_imgIndex);

		if (m_CastShadows)
		{
			m_ShadowScene.value().UpdateLightSpaceUniformBuffer(m_LightSystem, _imgIndex);
			m_Scene.SetLightSpaceDescriptor(m_ShadowScene.value().GetBufferDescriptorInfo(), _imgIndex);
			RenderShadowMap(cmdBuffer, _imgIndex);
		}

		ViewProjMatrix viewProjMatrix{ m_Camera.GetViewProjMatrix() };
		viewProjMatrix.m_Proj[1][1] *= -1.0f;
		m_Scene.UpdateVPUniformBuffers(viewProjMatrix, _imgIndex);

		uint8 lightCount{ 0 };
		auto lightData{ m_LightSystem.UpdateLightData(lightCount) };
		m_Scene.UpdateLightUniformBuffer(std::span{ lightData }.first(lightCount), _imgIndex);
		m_Scene.UpdateDescriptorSet(_imgIndex);

		RenderScene(cmdBuffer, _imgIndex);

		m_CommandBuffers.End(_imgIndex);
	}

	void VulkanRenderer::RenderShadowMap(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_ShadowScene.value().GetRenderPass().Get();
		renderPassInfo.framebuffer = m_ShadowScene.value().GetFramebuffer().Get();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_ShadowScene.value().GetShadowWidth(), m_ShadowScene.value().GetShadowHeight() });

		constexpr VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 1> clearAttachments{};
		clearAttachments.at(0).depthStencil = clearDepthStencil;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = clearAttachments.data();

		vkCmdBeginRenderPass(_cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_ShadowScene.value().GetShadowWidth());
		viewport.height = static_cast<float>(m_ShadowScene.value().GetShadowHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ m_ShadowScene.value().GetShadowWidth(), m_ShadowScene.value().GetShadowHeight() });
		vkCmdSetScissor(_cmdBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_ShadowScene.value().GetPipeline().Get());

		uint32 dummyOffset{ 0 };
		auto currentDescriptorSet{ m_ShadowScene.value().GetDescriptorSets().at(_imgIndex).Get() };
		vkCmdBindDescriptorSets(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_ShadowScene.value().GetPipeline().GetLayout(), 0, 1, &currentDescriptorSet, 1, &dummyOffset);

		for (const auto& mesh : m_MeshSystem.GetAllMeshes())
		{
			const glm::mat4 entityModelMatrix{ m_TransformationSystem.GetEntityModelMatrix(mesh.GetEntityId()) };

			const VulkanVertexBuffer* const vertexBuffer{ m_MeshSystem.GetVertexBuffer(mesh.GetVertexBufferId()) };
			const VkDeviceSize indexOffset{ mesh.GetIndexOffset() * sizeof(uint32) };
			vertexBuffer->Bind(_cmdBuffer, indexOffset);

			const glm::mat4& modelMatrix{ entityModelMatrix * mesh.GetModelMatrix() };
			vkCmdPushConstants(_cmdBuffer, m_ShadowScene.value().GetPipeline().GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);

			vkCmdDrawIndexed(_cmdBuffer, mesh.GetIndexCount(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(_cmdBuffer);
	}

	void VulkanRenderer::RenderScene(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Scene.GetRenderPass().Get();
		renderPassInfo.framebuffer = m_Scene.GetFramebuffer(_imgIndex).Get();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight() });

		constexpr VkClearColorValue clearColor{ 0.1f, 0.1f, 0.1f, 1.0f };
		constexpr VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 2> clearAttachments{};
		clearAttachments.at(0).color = clearColor;
		clearAttachments.at(1).depthStencil = clearDepthStencil;
		renderPassInfo.clearValueCount = static_cast<uint32>(clearAttachments.size());
		renderPassInfo.pClearValues = clearAttachments.data();

		vkCmdBeginRenderPass(_cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_RenderContext.GetSwapchain().GetWidth());
		viewport.height = static_cast<float>(m_RenderContext.GetSwapchain().GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight() });
		vkCmdSetScissor(_cmdBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Scene.GetStandardPipeline().Get());

		for (const auto& mesh : m_MeshSystem.GetAllMeshes())
		{
			const glm::mat4 entityModelMatrix{ m_TransformationSystem.GetEntityModelMatrix(mesh.GetEntityId()) };

			const VulkanVertexBuffer* const vertexBuffer{ m_MeshSystem.GetVertexBuffer(mesh.GetVertexBufferId()) };
			const VkDeviceSize indexOffset{ mesh.GetIndexOffset() * sizeof(uint32) };
			vertexBuffer->Bind(_cmdBuffer, indexOffset);

			const uint32 dynamicOffset{ static_cast<uint32>(m_Scene.GetMaterialMemoryAlignment()) * mesh.GetMeshId() };
			auto currentDescriptorSet{ m_Scene.GetDescriptorSets().at(_imgIndex).Get() };
			vkCmdBindDescriptorSets(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Scene.GetStandardPipeline().GetLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

			const glm::mat4& modelMatrix{ entityModelMatrix * mesh.GetModelMatrix() };
			const PushConstant pc{ modelMatrix, m_Camera.GetPosition(), mesh.GetTexId() };
			vkCmdPushConstants(_cmdBuffer, m_Scene.GetStandardPipeline().GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pc);

			vkCmdDrawIndexed(_cmdBuffer, mesh.GetIndexCount(), 1, 0, 0, 0);
		}

		RenderCubemap(_cmdBuffer, _imgIndex);

		vkCmdEndRenderPass(_cmdBuffer);
	}

	void VulkanRenderer::RenderCubemap(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex)
	{
		const MeshData& cubemap{ m_MeshSystem.GetSpecialMesh(m_CubemapMeshId) };
		const uint32 vertBufferId{ cubemap.GetVertexBufferId() };
		const VulkanVertexBuffer* const skyboxVertexBuffer{ m_MeshSystem.GetVertexBuffer(vertBufferId) };
		skyboxVertexBuffer->Bind(_cmdBuffer, cubemap.GetIndexOffset() * sizeof(uint32));

		vkCmdBindPipeline(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Scene.GetSkyboxPipeline().Get());

		auto currentDescriptorSet{ m_Scene.GetDescriptorSetsCubemap().at(_imgIndex).Get() };
		vkCmdBindDescriptorSets(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Scene.GetSkyboxPipeline().GetLayout(), 0, 1, &currentDescriptorSet, 0, nullptr);

		vkCmdDrawIndexed(_cmdBuffer, cubemap.GetIndexCount(), 1, 0, 0, 0);
	}
} // End of namespace