#pragma once

#include "Foundation/Platform.h"
#include <vector>
#include <array>
#include <string>

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

	class VulkanTextureManager
	{
	public:
		VulkanTextureManager(const VulkanDevice& _device, const VkCommandPool& _commandPool);
		~VulkanTextureManager();

		VulkanTextureManager(const VulkanTextureManager&) = delete;
		VulkanTextureManager& operator=(const VulkanTextureManager&) = delete;
		VulkanTextureManager(VulkanTextureManager&&) = delete;
		VulkanTextureManager& operator=(VulkanTextureManager&&) = delete;

		void CreateCubemap(const std::array<std::string, 6>& _filePaths);
		const std::vector<VkImageView>& GetTextureImageViews() const noexcept { return m_TextureImageViews; }
		const VkImageView& GetDummyDepthTexture();
		const VkImageView& GetCubemapTexture() const noexcept { return m_CubemapImageView; }

	private:
		void CreateStagingBuffer(const uint64 _sizeOfBuffer, const unsigned char* _pixels, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory) const;
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
		VkImage m_CubemapImage;
		VkImageView m_CubemapImageView;
	};
} // End of namespace