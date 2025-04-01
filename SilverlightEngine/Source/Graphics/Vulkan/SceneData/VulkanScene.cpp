#include "VulkanScene.h"
#include "Graphics/Vulkan/VulkanRenderContext.h"
#include "Graphics/DataStructs.h"
#include "Graphics/Material.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/MeshData.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	constexpr static uint64 g_MaxEntities{ 512 };

	VulkanScene::VulkanScene(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags) :
		m_RenderContext{ _renderContext },
		m_DepthBuffer{ _renderContext.GetDevice(), _renderContext.GetSwapchain().GetWidth(), _renderContext.GetSwapchain().GetHeight(), _flags },
		m_RenderPass{ _renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetSwapchain().GetFormat(), m_DepthBuffer.GetFormat() },
		m_DescriptorSetLayout{ _renderContext.GetDevice().GetLogicalDevice() },
		m_Pipeline{ _renderContext, m_DescriptorSetLayout.Get(), m_RenderPass.Get() },
		m_MaterialDynamicBufferMemAlignment{ 0 },
		m_MaterialDynamicBufferMemBlock{ nullptr, [](Material* _ptr) noexcept { _aligned_free(_ptr); } },
		m_NumOfSwapChainImages{ _renderContext.GetSwapchain().GetImageViews().size() }
	{
		AllocateDynamicBufferSpace();
		CreateDescriptorSetWriteBufferProperties();

		m_Framebuffers.reserve(m_NumOfSwapChainImages);

		for (size_t i = 0; i < m_NumOfSwapChainImages; ++i)
		{
			m_Framebuffers.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), m_RenderPass.Get(), _renderContext.GetSwapchain().GetImageViews().at(i), m_DepthBuffer.GetImageView(), _renderContext.GetSwapchain().GetWidth(), _renderContext.GetSwapchain().GetHeight());
		}

		m_UniformBuffers[UniformBufferType::VP].reserve(m_NumOfSwapChainImages);
		m_UniformBuffers[UniformBufferType::Material].reserve(m_NumOfSwapChainImages);
		m_UniformBuffers[UniformBufferType::Light].reserve(m_NumOfSwapChainImages);
		m_DescriptorSets.reserve(m_NumOfSwapChainImages);

		const VkDevice& logicalDevice{ _renderContext.GetDevice().GetLogicalDevice() };
		const VkPhysicalDevice& gpu{ _renderContext.GetDevice().GetPhysicalDevice() };

		for (size_t i = 0; i < m_NumOfSwapChainImages; ++i)
		{
			m_UniformBuffers[UniformBufferType::VP].emplace_back(logicalDevice, gpu, sizeof(ViewProjMatrix), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_UniformBuffers[UniformBufferType::Material].emplace_back(logicalDevice, gpu, m_MaterialDynamicBufferMemAlignment * g_MaxEntities, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_UniformBuffers[UniformBufferType::Light].emplace_back(logicalDevice, gpu, sizeof(LightBuffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			m_DescriptorSets.emplace_back(logicalDevice, _descriptorPool, m_DescriptorSetLayout.Get());
		}
	}

	void VulkanScene::AllocateDynamicBufferSpace() noexcept
	{
		const VkDeviceSize minUniformBufferOffset{ m_RenderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment };
		m_MaterialDynamicBufferMemAlignment = (sizeof(Material) + minUniformBufferOffset - 1) & ~(m_RenderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment - 1);
		m_MaterialDynamicBufferMemBlock.reset(static_cast<Material*>(_aligned_malloc(m_MaterialDynamicBufferMemAlignment * g_MaxEntities, m_MaterialDynamicBufferMemAlignment)));
	}

	void VulkanScene::CreateDescriptorSetWriteBufferProperties()
	{
		 m_DescriptorSetWriters = 
		 {
		 	{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER },
		 	{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC },
		 	{ 4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER },
		 	{ 5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER }
		 };
		 
		 m_DescriptorSetTextureWriters = 
		 {
		 	{ 2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE },
		 	{ 3, VK_DESCRIPTOR_TYPE_SAMPLER },
		 	{ 6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER }
		 };
	}

	void VulkanScene::UpdateDescriptorSets(const uint32 _imgIndex)
	{
		auto& descriptorSet{ m_DescriptorSets.at(_imgIndex) };

		m_DescriptorSetWriters.at(static_cast<size_t>(UniformBufferType::VP)).SetBuffer
		(
			m_UniformBuffers[UniformBufferType::VP].at(_imgIndex).GetBuffer(),
			m_UniformBuffers[UniformBufferType::VP].at(_imgIndex).GetBufferSize()
		);

		m_DescriptorSetWriters.at(static_cast<size_t>(UniformBufferType::Material)).SetBuffer
		(
			m_UniformBuffers[UniformBufferType::Material].at(_imgIndex).GetBuffer(),
			m_MaterialDynamicBufferMemAlignment
		);

		m_DescriptorSetWriters.at(static_cast<size_t>(UniformBufferType::Light)).SetBuffer
		(
			m_UniformBuffers[UniformBufferType::Light].at(_imgIndex).GetBuffer(),
			m_UniformBuffers[UniformBufferType::Light].at(_imgIndex).GetBufferSize()
		);

		descriptorSet.UpdateDescriptorSet(m_DescriptorSetWriters);
	}

	void VulkanScene::UpdateLightSpaceUniformBuffers(const std::vector<VulkanUniformBuffer>& _lightSpaceUniformBuffers, const uint32 _imgIndex)
	{
		const auto buffer{ _lightSpaceUniformBuffers.at(_imgIndex).GetBuffer() };
		const auto bufferSize{ _lightSpaceUniformBuffers.at(_imgIndex).GetBufferSize() };
		m_DescriptorSetWriters.at(static_cast<size_t>(UniformBufferType::LightSpaceMatrix)).SetBuffer(buffer, bufferSize);
	}

	void VulkanScene::UpdateMeshUniformBuffers(const MeshSystem& _meshSystem)
	{
		for (const auto& subMesh : _meshSystem.GetAllSubMeshes())
		{
			Material* materialData{ (Material*)((uint64)m_MaterialDynamicBufferMemBlock.get() + (subMesh.GetMeshId() * m_MaterialDynamicBufferMemAlignment)) };
			*materialData = { subMesh.GetMaterial().GetDiffuseColor(), subMesh.GetMaterial().GetSpecularColor() };

			for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
			{
				m_UniformBuffers[UniformBufferType::Material].at(i).CopyData(m_MaterialDynamicBufferMemBlock.get());
			}
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
		m_DescriptorSetTextureWriters.at(static_cast<size_t>(TextureUniformBufferType::Texture)).SetImageViews(_textureViews);
		m_DescriptorSetTextureWriters.at(static_cast<size_t>(TextureUniformBufferType::Sampler)).SetSampler(_sampler);
	}

	void VulkanScene::SetSceneShadowMap(const VkImageView& _shadowMapView, const VkSampler& _sampler)
	{
		m_DescriptorSetTextureWriters.at(static_cast<size_t>(TextureUniformBufferType::Combined)).SetImageViews({ _shadowMapView });
		m_DescriptorSetTextureWriters.at(static_cast<size_t>(TextureUniformBufferType::Combined)).SetSampler(_sampler);
	}

	void VulkanScene::RecreateResources(const uint32 _w, const uint32 _h, const uint32 _flags)
	{
		m_DepthBuffer.RecreateDepthBuffer(_w, _h, _flags);

		for (size_t i = 0; i < m_Framebuffers.size(); ++i)
		{
			m_Framebuffers.at(i).RecreateFramebuffer(_w, _h, m_RenderContext.GetSwapchain().GetImageViews().at(i), m_DepthBuffer.GetImageView());
		}
	}

	void VulkanScene::UpdateStaticDescriptorSets()
	{
		for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
		{
			m_DescriptorSets.at(i).UpdateDescriptorSet(m_DescriptorSetTextureWriters);
		}
	}
} // End of namespace