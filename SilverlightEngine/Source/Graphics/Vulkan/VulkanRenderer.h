#pragma once

#include "VulkanRenderContext.h"
#include "VulkanDescriptorPool.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanTextureSampler.h"
#include "VulkanTextureManager.h"
#include "VulkanVertexBufferManager.h"
#include "Graphics/Camera.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/Systems/TransformationSystem.h"
#include "SceneData/VulkanScene.h"
#include "SceneData/VulkanSceneShadow.h"

namespace Silverlight
{
	class VulkanRenderer
	{
	public:
		explicit VulkanRenderer(const Window& _window);
		~VulkanRenderer();

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) = delete;
		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

		void DrawFrame(const double _deltaTime);
		Camera& GetCamera() noexcept { return m_Camera; }

	private:
		void LoadDefaultCubemap();
		void InitializeGraphicsComponents();
		void RecordRenderCommands(const uint32 _imgIndex);
		void RenderShadowMap(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex);
		void RenderScene(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex);
		void RenderCubemap(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex);

	private:
		VulkanRenderContext m_RenderContext;
		VulkanDescriptorPool m_DescriptorPool;
		VulkanScene m_Scene;
		VulkanCommandPool m_CommandPool;
		VulkanCommandBuffer m_CommandBuffers;
		VulkanSemaphore m_Semaphore;
		VulkanFence m_InFlightFences;
		VulkanTextureSampler m_TextureSampler;
		VulkanTextureSampler m_CubemapSampler;
		VulkanTextureManager m_TextureManager;
		VulkanVertexBufferManager m_VertexBufferManager;
		Camera m_Camera;
		MeshSystem m_MeshSystem;
		LightSystem m_LightSystem;
		TransformationSystem m_TransformationSystem;
		std::optional<VulkanSceneShadow> m_ShadowScene;
		bool m_CastShadows;
		uint32 m_CubemapMeshId;
		std::vector<size_t> m_ImagesInFlight;
		size_t m_MaxFramesInFlight;
	};
} // End of namespace