#pragma once

#include "VulkanRenderContext.h"
#include "VulkanDescriptorPool.h"
#include "SceneData/VulkanScene.h"
#include "SceneData/VulkanSceneShadow.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanTextureSampler.h"
#include "VulkanTexture.h"
#include "VulkanVertexBufferManager.h"
#include "Graphics/Camera.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/Systems/TransformationSystem.h"

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

	private:
		void InitializeGraphicsComponents();
		void RecordRenderCommands(const uint32 _imgIndex);
		void RenderShadowMap(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex);
		void RenderScene(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex);

	private:
		VulkanRenderContext m_RenderContext;
		VulkanDescriptorPool m_DescriptorPool;
		VulkanScene m_Scene;
		std::optional<VulkanSceneShadow> m_ShadowScene;
		VulkanCommandPool m_CommandPool;
		VulkanCommandBuffer m_CommandBuffers;
		VulkanSemaphore m_Semaphore;
		VulkanFence m_InFlightFences;
		VulkanTextureSampler m_TextureSampler;
		VulkanTexture m_Textures;
		VulkanVertexBufferManager m_VertexBufferManager;
		Camera m_Camera;
		MeshSystem m_MeshSystem;
		LightSystem m_LightSystem;
		TransformationSystem m_TransformationSystem;
		uint32 m_CurrentFrameIndex;
		bool m_CastShadows;
	};
} // End of namespace