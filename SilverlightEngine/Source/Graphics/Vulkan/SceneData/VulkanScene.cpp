#include "VulkanScene.h"
#include "Graphics/Vulkan/VulkanRenderContext.h"
#include "Graphics/Vulkan/VulkanDescriptorInfo.h"
#include "Graphics/DataStructs.h"
#include "Graphics/Material.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/MeshData.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	// Internal linkage
	constexpr uint64 g_MaxEntities{ 512 };
	constexpr std::array<DescriptorSetLayoutBinding, 7> g_SceneBindings
	{ {
		{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT },
		{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, VK_SHADER_STAGE_FRAGMENT_BIT },
		{ 2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 16, VK_SHADER_STAGE_FRAGMENT_BIT },
		{ 3, VK_DESCRIPTOR_TYPE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT },
		{ 4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT },
		{ 5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT },
		{ 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT }
	} };
	constexpr std::array<DescriptorSetLayoutBinding, 2> g_CubemapBindings
	{ {
		{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT },
		{ 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT }
	} };

	VulkanScene::VulkanScene(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags) :
		m_RenderContext{ _renderContext },
		m_DepthBuffer{ _renderContext.GetDevice(), _renderContext.GetSwapchain().GetWidth(), _renderContext.GetSwapchain().GetHeight(), _flags },
		m_RenderPass{ _renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetSwapchain().GetFormat(), m_DepthBuffer.GetFormat() },
		m_DescriptorSetLayoutScene{ _renderContext.GetDevice().GetLogicalDevice(), std::span{ g_SceneBindings } },
		m_DescriptorSetLayoutCubemap{ _renderContext.GetDevice().GetLogicalDevice(), std::span{ g_CubemapBindings } },
		m_StandardPipeline{ _renderContext, m_DescriptorSetLayoutScene.Get(), m_RenderPass.Get() },
		m_SkyboxPipeline{ _renderContext, m_DescriptorSetLayoutCubemap.Get(), m_RenderPass.Get() },
		m_MaterialDynamicBufferMemAlignment{ 0 },
		m_MaterialDynamicBufferMemBlock{ nullptr, [](Material* _ptr) noexcept { _aligned_free(_ptr); } },
		m_NumOfSwapChainImages{ _renderContext.GetSwapchain().GetImageViews().size() }
	{
		AllocateDynamicBufferSpace();

		m_Framebuffers.reserve(m_NumOfSwapChainImages);
		m_UniformBuffers[UniformBufferType::VP].reserve(m_NumOfSwapChainImages);
		m_UniformBuffers[UniformBufferType::Material].reserve(m_NumOfSwapChainImages);
		m_UniformBuffers[UniformBufferType::Light].reserve(m_NumOfSwapChainImages);
		m_DescriptorSets.reserve(m_NumOfSwapChainImages);
		m_DescriptorSetsCubemap.reserve(m_NumOfSwapChainImages);

		const VkDevice& logicalDevice{ _renderContext.GetDevice().GetLogicalDevice() };
		const VkPhysicalDevice& gpu{ _renderContext.GetDevice().GetPhysicalDevice() };

		for (size_t i = 0; i < m_NumOfSwapChainImages; ++i)
		{
			m_Framebuffers.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), m_RenderPass.Get(), _renderContext.GetSwapchain().GetImageViews().at(i), m_DepthBuffer.GetImageView(), _renderContext.GetSwapchain().GetWidth(), _renderContext.GetSwapchain().GetHeight());

			m_UniformBuffers[UniformBufferType::VP].emplace_back(logicalDevice, gpu, sizeof(ViewProjMatrix), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_UniformBuffers[UniformBufferType::Material].emplace_back(logicalDevice, gpu, m_MaterialDynamicBufferMemAlignment * g_MaxEntities, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_UniformBuffers[UniformBufferType::Light].emplace_back(logicalDevice, gpu, sizeof(LightBuffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			m_DescriptorSets.emplace_back(logicalDevice, _descriptorPool, m_DescriptorSetLayoutScene.Get());
			m_DescriptorSetsCubemap.emplace_back(logicalDevice, _descriptorPool, m_DescriptorSetLayoutCubemap.Get());
		}
	}

	void VulkanScene::AllocateDynamicBufferSpace() noexcept
	{
		const VkDeviceSize minUniformBufferOffset{ m_RenderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment };
		m_MaterialDynamicBufferMemAlignment = (sizeof(Material) + minUniformBufferOffset - 1) & ~(m_RenderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment - 1);
		m_MaterialDynamicBufferMemBlock.reset(static_cast<Material*>(_aligned_malloc(m_MaterialDynamicBufferMemAlignment * g_MaxEntities, m_MaterialDynamicBufferMemAlignment)));
	}

	void VulkanScene::UpdateDescriptorSet(const uint32 _imgIndex)
	{
		auto& descriptorSet{ m_DescriptorSets.at(_imgIndex) };
		auto& descriptorSetCubemap{ m_DescriptorSetsCubemap.at(_imgIndex) };

		VulkanBufferDescriptorInfo vpDescriptor{};
		vpDescriptor.m_Binding = 0;
		vpDescriptor.m_Buffer = m_UniformBuffers[UniformBufferType::VP].at(_imgIndex).GetBuffer();
		vpDescriptor.m_Size = m_UniformBuffers[UniformBufferType::VP].at(_imgIndex).GetBufferSize();
		vpDescriptor.m_Type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSet.UpdateDescriptorSet(vpDescriptor);
		descriptorSetCubemap.UpdateDescriptorSet(vpDescriptor);

		VulkanBufferDescriptorInfo matDescriptor{};
		matDescriptor.m_Binding = 1;
		matDescriptor.m_Buffer = m_UniformBuffers[UniformBufferType::Material].at(_imgIndex).GetBuffer();
		matDescriptor.m_Size = m_MaterialDynamicBufferMemAlignment;
		matDescriptor.m_Type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		descriptorSet.UpdateDescriptorSet(matDescriptor);

		VulkanBufferDescriptorInfo lightDescriptor{};
		lightDescriptor.m_Binding = 4;
		lightDescriptor.m_Buffer = m_UniformBuffers[UniformBufferType::Light].at(_imgIndex).GetBuffer();
		lightDescriptor.m_Size = m_UniformBuffers[UniformBufferType::Light].at(_imgIndex).GetBufferSize();
		lightDescriptor.m_Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorSet.UpdateDescriptorSet(lightDescriptor);
	}

	void VulkanScene::SetLightSpaceDescriptor(const VulkanBufferDescriptorInfo& _bufferDescriptorInfo, const uint32 _imgIndex)
	{
		auto& descriptorSet{ m_DescriptorSets.at(_imgIndex) };

		VulkanBufferDescriptorInfo lightSpaceDescriptor{};
		lightSpaceDescriptor.m_Binding = _bufferDescriptorInfo.m_Binding;
		lightSpaceDescriptor.m_Buffer = _bufferDescriptorInfo.m_Buffer;
		lightSpaceDescriptor.m_Size = _bufferDescriptorInfo.m_Size;
		lightSpaceDescriptor.m_Type = _bufferDescriptorInfo.m_Type;
		descriptorSet.UpdateDescriptorSet(lightSpaceDescriptor);
	}

	void VulkanScene::UpdateMaterialUniformBuffers(const MeshSystem& _meshSystem)
	{
		for (const auto& mesh : _meshSystem.GetAllMeshes())
		{
			Material* materialData{ (Material*)((uint64)m_MaterialDynamicBufferMemBlock.get() + (mesh.GetMeshId() * m_MaterialDynamicBufferMemAlignment)) };
			*materialData = { mesh.GetMaterial().GetDiffuseColor(), mesh.GetMaterial().GetSpecularColor() };
		}

		for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
		{
			m_UniformBuffers[UniformBufferType::Material].at(i).CopyData(m_MaterialDynamicBufferMemBlock.get());
		}
	}

	void VulkanScene::UpdateVPUniformBuffers(ViewProjMatrix& _viewProj, const uint32 _imgIndex) const
	{
		auto it{ m_UniformBuffers.find(UniformBufferType::VP) };
		if (it != m_UniformBuffers.end())
		{
			it->second.at(_imgIndex).CopyData(&_viewProj);
		}
	}

	void VulkanScene::UpdateLightUniformBuffer(std::span<const LightData> _lightData, const uint32 _imgIndex)
	{
		LightBuffer lightBuffer{};
		lightBuffer.m_TotalLights = static_cast<uint8>(_lightData.size());
		std::copy(_lightData.begin(), _lightData.end(), lightBuffer.m_Lights);

		auto it{ m_UniformBuffers.find(UniformBufferType::Light) };
		if (it != m_UniformBuffers.end())
		{
			it->second.at(_imgIndex).CopyData(&lightBuffer);
		}
	}

	void VulkanScene::SetSceneTextures(const std::vector<VkImageView>& _textureViews, const VkSampler& _sampler)
	{
		for (auto& descriptorSet : m_DescriptorSets)
		{
			VulkanTextureDescriptorInfo texDescriptor{};
			texDescriptor.m_Binding = 2;
			texDescriptor.m_ImageViews = _textureViews;
			texDescriptor.m_Sampler = VK_NULL_HANDLE;
			texDescriptor.m_Type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			texDescriptor.m_ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			descriptorSet.UpdateDescriptorSet(texDescriptor);

			VulkanTextureDescriptorInfo samplerDescriptor{};
			samplerDescriptor.m_Binding = 3;
			samplerDescriptor.m_ImageViews = {};
			samplerDescriptor.m_Sampler = _sampler;
			samplerDescriptor.m_Type = VK_DESCRIPTOR_TYPE_SAMPLER;
			samplerDescriptor.m_ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			descriptorSet.UpdateDescriptorSet(samplerDescriptor);
		}
	}

	void VulkanScene::SetSceneCubemap(const VkImageView& _cubemapView, const VkSampler& _sampler)
	{
		for (auto& descriptorSet : m_DescriptorSetsCubemap)
		{
			VulkanTextureDescriptorInfo cubemapDescriptor{};
			cubemapDescriptor.m_Binding = 1;
			cubemapDescriptor.m_ImageViews = { _cubemapView };
			cubemapDescriptor.m_Sampler = _sampler;
			cubemapDescriptor.m_Type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			cubemapDescriptor.m_ImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			descriptorSet.UpdateDescriptorSet(cubemapDescriptor);
		}
	}

	void VulkanScene::SetSceneShadowMap(const VkImageView& _shadowMapView, const VkSampler& _sampler)
	{
		for (auto& descriptorSet : m_DescriptorSets)
		{
			VulkanTextureDescriptorInfo shadowMapDescriptor{};
			shadowMapDescriptor.m_Binding = 6;
			shadowMapDescriptor.m_ImageViews = { _shadowMapView };
			shadowMapDescriptor.m_Sampler = _sampler;
			shadowMapDescriptor.m_Type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			shadowMapDescriptor.m_ImageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			descriptorSet.UpdateDescriptorSet(shadowMapDescriptor);
		}
	}

	void VulkanScene::RecreateResources(const uint32 _w, const uint32 _h, const uint32 _flags)
	{
		m_DepthBuffer.RecreateDepthBuffer(_w, _h, _flags);

		for (size_t i = 0; i < m_Framebuffers.size(); ++i)
		{
			m_Framebuffers.at(i).RecreateFramebuffer(_w, _h, m_RenderContext.GetSwapchain().GetImageViews().at(i), m_DepthBuffer.GetImageView());
		}
	}
} // End of namespace