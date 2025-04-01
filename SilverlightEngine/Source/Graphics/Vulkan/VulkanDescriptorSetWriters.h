#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkBuffer_T* VkBuffer;
typedef struct VkImageView_T* VkImageView;
typedef struct VkSampler_T* VkSampler;
typedef enum VkDescriptorType VkDescriptorType;

namespace Silverlight
{
	class VulkanDescriptorSetWriterBase
	{
	public:
		explicit VulkanDescriptorSetWriterBase(const uint32 _binding, const VkDescriptorType _descriptorType) noexcept :
			m_Binding{ _binding },
			m_DescriptorType{ _descriptorType }
		{}
		
		uint32 GetBinding() const noexcept { return m_Binding; }
		const VkDescriptorType& GetDescriptorType() const noexcept { return m_DescriptorType; }

	protected:
		uint32 m_Binding;
		VkDescriptorType m_DescriptorType;
	};

	class VulkanDescriptorSetBufferWriter : public VulkanDescriptorSetWriterBase
	{
	public:
		VulkanDescriptorSetBufferWriter(const uint32 _binding, const VkDescriptorType _descriptorType) noexcept :
			VulkanDescriptorSetWriterBase{ _binding, _descriptorType },
			m_Buffer{ nullptr },
			m_BufferRange{ 0 }
		{}
		
		void SetBuffer(const VkBuffer _buffer, const uint64 _bufferRange) noexcept
		{
			m_Buffer = _buffer;
			m_BufferRange = _bufferRange;
		}

		const VkBuffer& GetBuffer() const noexcept { return m_Buffer; }
		uint64 GetBufferRange() const noexcept { return m_BufferRange; }

	private:
		VkBuffer m_Buffer;
		uint64 m_BufferRange;
	};

	class VulkanDescriptorSetTextureWriter : public VulkanDescriptorSetWriterBase
	{
	public:
		VulkanDescriptorSetTextureWriter(const uint32 _binding, const VkDescriptorType _descriptorType) noexcept : 
			VulkanDescriptorSetWriterBase{ _binding, _descriptorType }, 
			m_Sampler{ nullptr }
		{}
		
		void SetImageViews(std::vector<VkImageView> _imageViews) noexcept
		{
			m_ImageViews = std::move(_imageViews);
		}
		
		void SetSampler(VkSampler _sampler) noexcept
		{
			m_Sampler = _sampler;
		}
		
		const std::vector<VkImageView>& GetImageViews() const noexcept { return m_ImageViews; }
		const VkSampler& GetSampler() const noexcept { return m_Sampler; }
	
	private:
		std::vector<VkImageView> m_ImageViews;
		VkSampler m_Sampler;
	};
} // End of namespace