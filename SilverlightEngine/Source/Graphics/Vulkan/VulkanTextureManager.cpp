#include "VulkanTextureManager.h"
#include "VulkanUtils.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/Logging/Logger.h"
#include "VulkanDevice.h"
#include <vulkan/vulkan_core.h>

namespace Silverlight
{
	VulkanImage::VulkanImage() noexcept : 
		m_Image{ VK_NULL_HANDLE },
		m_ImageView{ VK_NULL_HANDLE },
		m_ImageMemory{ VK_NULL_HANDLE }
	{}

	VulkanTextureManager::VulkanTextureManager(const VulkanDevice& _device, const VkCommandPool& _commandPool) :
		m_LogicalDevice{ _device.GetLogicalDevice() },
		m_PhysicalDevice{ _device.GetPhysicalDevice() },
		m_GraphicsQueue{ _device.GetGraphicsQueue() },
		m_CommandPool{ _commandPool },
		m_TextureImageFormat{ VK_FORMAT_R8G8B8A8_SRGB },
		m_TextureImages{},
		m_TextureImageViews{},
		m_DummyDepthImage{},
		m_CubemapImageView{}
	{
		UploadTextures();
	}

	VulkanTextureManager::~VulkanTextureManager()
	{
		if (m_DummyDepthImage.m_Image != VK_NULL_HANDLE)
		{
			vkDestroyImageView(m_LogicalDevice, m_DummyDepthImage.m_ImageView, nullptr);
			vkDestroyImage(m_LogicalDevice, m_DummyDepthImage.m_Image, nullptr);
			vkFreeMemory(m_LogicalDevice, m_DummyDepthImage.m_ImageMemory, nullptr);
		}

		for (const auto& image : m_TextureImages)
		{
			vkDestroyImageView(m_LogicalDevice, image.m_ImageView, nullptr);
			vkDestroyImage(m_LogicalDevice, image.m_Image, nullptr);
			vkFreeMemory(m_LogicalDevice, image.m_ImageMemory, nullptr);
		}
	}

	void VulkanTextureManager::UploadTextures()
	{
		const std::vector<Image>& images{ g_ResourceManager.GetImages() };
		m_TextureImages.reserve(images.size());
		m_TextureImageViews.reserve(images.size());

		for (const auto& image : images)
		{
			VkBuffer stagingBuffer{ VK_NULL_HANDLE };
			VkDeviceMemory stagingBufferMemory{ VK_NULL_HANDLE };

			CreateStagingBuffer(image.m_ImageSize, image.m_Pixels, stagingBuffer, stagingBufferMemory);
			CreateTextureImage(stagingBuffer, image.m_ImageWidth, image.m_ImageHeight);

			vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
			vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
			stagingBufferMemory = VK_NULL_HANDLE;
			stagingBuffer = VK_NULL_HANDLE;
		}
	}

	void VulkanTextureManager::CreateCubemap(const std::array<std::string, 6>& _filePaths)
	{
		std::array<uint16, 6> skyboxImgIndices{};

		for (size_t i = 0; i < skyboxImgIndices.size(); ++i)
		{
			const uint16 imageIndex{ g_ResourceManager.LoadImageResource(_filePaths[i]) };
			skyboxImgIndices[i] = imageIndex;
		}

		const std::vector<Image>& images{ g_ResourceManager.GetImages() };

		VkDeviceMemory cubemapImageMemory{ VK_NULL_HANDLE };

		VulkanUtils::CreateImage
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			images[skyboxImgIndices[0]].m_ImageWidth, images[skyboxImgIndices[0]].m_ImageHeight,
			m_TextureImageFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlagBits(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_CubemapImage,
			cubemapImageMemory, 
			6,
			VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT
		);

		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			m_CubemapImage,
			m_TextureImageFormat,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			6
		);

		std::vector<VkBuffer> stagingBuffers{ 6 };
		std::vector<VkDeviceMemory> stagingBufferMemories{ 6 };

		for (size_t i = 0; i < 6; ++i)
		{
			uint16 imgIndex{ skyboxImgIndices[i] };
			VkBuffer stagingBuffer{ VK_NULL_HANDLE };
			VkDeviceMemory stagingBufferMemory{ VK_NULL_HANDLE };

			CreateStagingBuffer(images[imgIndex].m_ImageSize, images[imgIndex].m_Pixels, stagingBuffer, stagingBufferMemory);

			VulkanUtils::CopyBufferToImage
			(
				m_LogicalDevice,
				m_CommandPool,
				m_GraphicsQueue,
				stagingBuffer,
				m_CubemapImage,
				images[skyboxImgIndices[0]].m_ImageWidth, 
				images[skyboxImgIndices[0]].m_ImageHeight,
				static_cast<uint32>(i)
			);

			vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
			vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		}

		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			m_CubemapImage,
			m_TextureImageFormat,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			6
		);

		VulkanUtils::CreateImageView
		(
			m_LogicalDevice,
			m_CubemapImage,
			m_TextureImageFormat,
			VK_IMAGE_ASPECT_COLOR_BIT,
			m_CubemapImageView,
			6,
			VK_IMAGE_VIEW_TYPE_CUBE
		);

		m_TextureImages.emplace_back(m_CubemapImage, m_CubemapImageView, cubemapImageMemory);
		m_TextureImageViews.emplace_back(m_CubemapImageView);
		SE_LOG(LogCategory::Info, "[TEXTURE CUBE]: Created a cubemap (total textures: %d)", m_TextureImages.size());
	}

	const VkImageView& VulkanTextureManager::GetDummyDepthTexture()
	{
		if (m_DummyDepthImage.m_ImageView != VK_NULL_HANDLE)
		{
			return m_DummyDepthImage.m_ImageView;
		}

		const auto format{ VulkanUtils::FindSupportedFormat
		(
			m_PhysicalDevice,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		) };

		const VkImageUsageFlagBits usageFlags
		{
			static_cast<VkImageUsageFlagBits>((VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT))
		};

		VulkanUtils::CreateImage
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			1, 1,
			format,
			VK_IMAGE_TILING_OPTIMAL,
			usageFlags,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_DummyDepthImage.m_Image,
			m_DummyDepthImage.m_ImageMemory
		);

		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			m_DummyDepthImage.m_Image,
			format,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
		);

		VulkanUtils::CreateImageView
		(
			m_LogicalDevice,
			m_DummyDepthImage.m_Image,
			format,
			VK_IMAGE_ASPECT_DEPTH_BIT,
			m_DummyDepthImage.m_ImageView
		);

		return m_DummyDepthImage.m_ImageView;
	}

	void VulkanTextureManager::CreateStagingBuffer(const uint64 _sizeOfBuffer, const unsigned char* _pixels, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory) const
	{
		VulkanUtils::CreateBuffer
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			_sizeOfBuffer,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			_buffer,
			_bufferMemory
		);

		void* data{ nullptr };
		vkMapMemory(m_LogicalDevice, _bufferMemory, 0, _sizeOfBuffer, 0, &data);
		memcpy(data, _pixels, _sizeOfBuffer);
		vkUnmapMemory(m_LogicalDevice, _bufferMemory);
	}

	void VulkanTextureManager::CreateTextureImage(const VkBuffer& _buffer, const uint32 _imgW, const uint32 _imgH)
	{
		VkImage textureImage{};
		VkImageView textureImageView{};
		VkDeviceMemory textureImageMemory{};

		VulkanUtils::CreateImage
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			_imgW,
			_imgH,
			m_TextureImageFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlagBits(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			textureImage,
			textureImageMemory
		);

		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			textureImage,
			m_TextureImageFormat,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
		);

		VulkanUtils::CopyBufferToImage
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			_buffer,
			textureImage,
			_imgW,
			_imgH
		);

		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			textureImage,
			m_TextureImageFormat,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		VulkanUtils::CreateImageView(m_LogicalDevice, textureImage, m_TextureImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, textureImageView);
		m_TextureImages.emplace_back(textureImage, textureImageView, textureImageMemory);
		m_TextureImageViews.emplace_back(textureImageView);
		SE_LOG(LogCategory::Info, "[TEXTURE]: Created texture image object (total textures: %d)", m_TextureImages.size());
	}
} // End of namespace