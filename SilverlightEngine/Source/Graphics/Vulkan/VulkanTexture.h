#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkQueue_T* VkQueue;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef enum VkFormat VkFormat;

namespace Silverlight
{
	class VulkanDevice;

	struct VulkanImage
	{
		VulkanImage() noexcept;
		VulkanImage(const VkImage& _image, const VkImageView _imageView, const VkDeviceMemory& _imageMemory) noexcept :
			m_Image{ _image },
			m_ImageView{ _imageView },
			m_ImageMemory{ _imageMemory }
		{}

		VkImage m_Image;
		VkImageView m_ImageView;
		VkDeviceMemory m_ImageMemory;
	};

	class VulkanTexture
	{
	public:
		VulkanTexture(const VulkanDevice& _device, const VkCommandPool& _commandPool);
		~VulkanTexture();

		VulkanTexture(const VulkanTexture&) = delete;
		VulkanTexture& operator=(const VulkanTexture&) = delete;
		VulkanTexture(VulkanTexture&&) = delete;
		VulkanTexture& operator=(VulkanTexture&&) = delete;

		const std::vector<VkImageView>& GetTextureImageViews() const noexcept { return m_TextureImageViews; }
		const VkImageView& GetDummyDepthTexture();

	private:
		void CreateStagingBuffer(const uint64 _sizeOfBuffer, const unsigned char* _pixels, const uint32 _imgW, const uint32 _imgH);
		void CreateTextureImage(const VkBuffer& _buffer, const uint32 _imgW, const uint32 _imgH);
		void UploadTextures();

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkQueue m_GraphicsQueue;
		VkCommandPool m_CommandPool;
		VkFormat m_TextureImageFormat;
		std::vector<VulkanImage> m_TextureImages;
		std::vector<VkImageView> m_TextureImageViews;
		VulkanImage m_DummyDepthImage;
	};
} // End of namespace