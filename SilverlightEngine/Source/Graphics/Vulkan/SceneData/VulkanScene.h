#pragma once

#include "Graphics/Vulkan/VulkanDepthBuffer.h"
#include "Graphics/Vulkan/RenderPass/VulkanSceneRenderPass.h"
#include "Graphics/Vulkan/VulkanFramebuffer.h"
#include "Graphics/Vulkan/Layout/VulkanSceneDescriptorSetLayout.h"
#include "Graphics/Vulkan/Pipeline/VulkanGraphicsPipeline.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"
#include "Graphics/Vulkan/VulkanDescriptorSet.h"
#include "Graphics/Vulkan/VulkanDescriptorSetWriters.h"
#include "Graphics/Components/Light/LightData.h"
#include <unordered_map>
#include <memory>
#include <span> 

typedef struct VkSampler_T* VkSampler;

namespace Silverlight
{
	class VulkanRenderContext;
	class Material;
	class MeshSystem;
	struct ViewProjMatrix;

	enum class UniformBufferType : uint8
	{ 
		VP, 
		Material, 
		Light,
		LightSpaceMatrix
	};

	enum class TextureUniformBufferType : uint8
	{
		Texture,
		Sampler,
		Combined
	};

	class VulkanScene
	{
	public:
		VulkanScene(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags);

		VulkanScene(const VulkanScene&) = delete;
		VulkanScene& operator=(const VulkanScene&) = delete;
		VulkanScene(VulkanScene&&) = delete;
		VulkanScene& operator=(VulkanScene&&) = delete;

		void UpdateLightSpaceUniformBuffers(const std::vector<VulkanUniformBuffer>& _lightSpaceUniformBuffers, const uint32 _imgIndex);
		void UpdateMeshUniformBuffers(const MeshSystem& _meshSystem);
		void UpdateVPUniformBuffers(ViewProjMatrix& _viewProj, const uint32 _imgIndex) const;
		void UpdateLightUniformBuffer(std::span<const LightData> _lightData, const uint32 _imgIndex);
		void SetSceneTextures(const std::vector<VkImageView>& _textureViews, const VkSampler& _sampler);
		void SetSceneShadowMap(const VkImageView& _shadowMapView, const VkSampler& _sampler);
		void RecreateResources(const uint32 _w, const uint32 _h, const uint32 _flags);
		void UpdateStaticDescriptorSets();
		void UpdateDescriptorSets(const uint32 _imgIndex);
		const VkRenderPass& GetRenderPass() const noexcept { return m_RenderPass.Get(); }
		const VulkanFramebuffer& GetFramebuffer(const uint32 _index) const { return m_Framebuffers.at(_index); }
		const VulkanGraphicsPipeline& GetPipeline() const noexcept { return m_Pipeline; }
		std::vector<VulkanDescriptorSet>& GetDescriptorSets() noexcept { return m_DescriptorSets; }
		uint64 GetMaterialMemoryAlignment() const noexcept { return m_MaterialDynamicBufferMemAlignment; }

	private:
		void AllocateDynamicBufferSpace() noexcept;
		void CreateDescriptorSetWriteBufferProperties();

	private:
		const VulkanRenderContext& m_RenderContext;
		VulkanDepthBuffer m_DepthBuffer;
		VulkanSceneRenderPass m_RenderPass;
		std::vector<VulkanFramebuffer> m_Framebuffers;
		VulkanSceneDescriptorSetLayout m_DescriptorSetLayout;
		VulkanGraphicsPipeline m_Pipeline;
		std::vector<VulkanDescriptorSetBufferWriter> m_DescriptorSetWriters;
		std::vector<VulkanDescriptorSetTextureWriter> m_DescriptorSetTextureWriters;
		uint64 m_MaterialDynamicBufferMemAlignment;
		std::unique_ptr<Material, void(*)(Material*) noexcept> m_MaterialDynamicBufferMemBlock;
		size_t m_NumOfSwapChainImages;
		std::unordered_map<UniformBufferType, std::vector<VulkanUniformBuffer>> m_UniformBuffers;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
	};
} // End of namespace