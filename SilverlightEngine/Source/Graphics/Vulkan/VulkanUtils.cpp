#include "VulkanUtils.h"
#include <vulkan/vulkan_core.h>
#include <stdexcept>

namespace Silverlight
{
	void VulkanUtils::CheckInstanceExtSupport(const std::vector<const char*>& _requiredExtensions)
	{
		uint32 extensionCount{ 0 };
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		bool found{ false };

		for (int i = 0; i < _requiredExtensions.size(); ++i)
		{
			for (const auto& extension : extensions)
			{
				found = false;

				if (std::strcmp(_requiredExtensions[i], extension.extensionName) == 0)
				{
					found = true;
					break;
				}
			}
		}
	}

	void VulkanUtils::CheckInstanceLayerSupport(const std::vector<const char*>& _requiredLayers)
	{
		uint32 layerCount{ 0 };
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		bool found{ false };

		for (int i = 0; i < _requiredLayers.size(); ++i)
		{
			for (const auto& layer : layers)
			{
				found = false;

				if (std::strcmp(_requiredLayers[i], layer.layerName) == 0)
				{
					found = true;
					break;
				}
			}
		}
	}

	void VulkanUtils::CheckDeviceExtSupport(const VkPhysicalDevice& _gpu, const std::vector<const char*>& _requiredExtensions)
	{
		uint32 extensionCount{ 0 };
		vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &extensionCount, extensions.data());

		bool found{ false };

		for (int i = 0; i < _requiredExtensions.size(); ++i)
		{
			for (const auto& extension : extensions)
			{
				found = false;

				if (std::strcmp(_requiredExtensions[i], extension.extensionName) == 0)
				{
					found = true;
					break;
				}
			}
		}
	}

	VkShaderModule VulkanUtils::CreateShaderModule(const VkDevice& _logicalDevice, const std::vector<char>& _shaderBinaryCode)
	{
		VkShaderModuleCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderCreateInfo.codeSize = _shaderBinaryCode.size();
		shaderCreateInfo.pCode = reinterpret_cast<const uint32*>(_shaderBinaryCode.data());

		VkShaderModule shaderModule = VK_NULL_HANDLE;
		if (vkCreateShaderModule(_logicalDevice, &shaderCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a shader module");
		}

		return shaderModule;
	}

	void VulkanUtils::CreateImageView(const VkDevice& _logicalDevice, const VkImage& _image, const uint32 _format, const uint32 _aspect, VkImageView& _imageView)
	{
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = _image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = static_cast<VkFormat>(_format);
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewCreateInfo.subresourceRange.aspectMask = _aspect;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(_logicalDevice, &imageViewCreateInfo, nullptr, &_imageView) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a Vulkan image view object");
		}
	}

	void VulkanUtils::CreateBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint64 _size, const uint32 _usage, const uint32 _memoryPropertyFlags, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory)
	{
		// Create buffer object
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = _size;
		bufferCreateInfo.usage = _usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(_logicalDevice, &bufferCreateInfo, nullptr, &_buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a vertex buffer");
		}

		// Allocate memory for the buffer
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(_logicalDevice, _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanUtils::FindMemoryTypeIndex(_gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to allocated memory for a vertex buffer");
		}

		vkBindBufferMemory(_logicalDevice, _buffer, _bufferMemory, 0);
	}

	void VulkanUtils::CreateImage(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32 _w, const uint32 _h, const VkFormat _format, const VkImageTiling _tiling, const VkImageUsageFlagBits _usage, const uint32 _memoryPropertyFlags, VkImage& _image, VkDeviceMemory& _imageMemory)
	{
		// Create image object
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.extent.width = _w;
		imageCreateInfo.extent.height = _h;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.format = _format;
		imageCreateInfo.tiling = _tiling;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = _usage;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(_logicalDevice, &imageCreateInfo, nullptr, &_image) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create image");
		}

		// Allocate memory for image
		VkMemoryRequirements memReq{};
		vkGetImageMemoryRequirements(_logicalDevice, _image, &memReq);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReq.size;
		allocInfo.memoryTypeIndex = FindMemoryTypeIndex(_gpu, memReq.memoryTypeBits, _memoryPropertyFlags);

		if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr, &_imageMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to allocate memory for image");
		}

		// Connect memory to image
		vkBindImageMemory(_logicalDevice, _image, _imageMemory, 0);
	}

	uint32 VulkanUtils::FindMemoryTypeIndex(const VkPhysicalDevice& _gpu, const uint32 _memoryTypeBits, const uint32 _memoryPropertyFlags) noexcept
	{
		VkPhysicalDeviceMemoryProperties memoryProperties{};
		vkGetPhysicalDeviceMemoryProperties(_gpu, &memoryProperties);

		for (uint32 i = 0; i < memoryProperties.memoryTypeCount; ++i)
		{
			if ((_memoryTypeBits & (1 << i)) &&
				(memoryProperties.memoryTypes[i].propertyFlags & _memoryPropertyFlags) == _memoryPropertyFlags)
			{
				return i;
			}
		}

		return UINT32_MAX;
	}

	void VulkanUtils::CopyBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, const uint64 _size, const VkBuffer& _srcBuffer, const VkBuffer& _dstBuffer)
	{
		VkCommandBuffer cmdBuffer = BeginSingleTimeCommands(_logicalDevice, _commandPool, _queue);

		VkBufferCopy bufferCopy{};
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = 0;
		bufferCopy.size = _size;
		vkCmdCopyBuffer(cmdBuffer, _srcBuffer, _dstBuffer, 1, &bufferCopy);

		EndSingleTimeCommands(_logicalDevice, _commandPool, _queue, cmdBuffer);
	}

	VkFormat VulkanUtils::FindSupportedFormat(const VkPhysicalDevice& _gpu, const std::vector<VkFormat>& _formats, const VkImageTiling _tiling, const uint32 _formatFeatures)
	{
		for (const VkFormat format : _formats)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(_gpu, format, &props);

			if (_tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & _formatFeatures) == _formatFeatures)
			{
				return format;
			}
			else if (_tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & _formatFeatures) == _formatFeatures)
			{
				return format;
			}
		}

		throw std::runtime_error("ERROR: Failed to find supported format");
	}

	constexpr bool VulkanUtils::HasStencilComponent(const VkFormat _format) noexcept
	{
		return _format == VK_FORMAT_D32_SFLOAT_S8_UINT || _format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void VulkanUtils::TransitionImageLayout(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, VkImage& _image, const VkFormat _imageFormat, const VkImageLayout _oldLayout, const VkImageLayout _newLayout)
	{
		VkCommandBuffer cmdBuffer = BeginSingleTimeCommands(_logicalDevice, _commandPool, _queue);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = _oldLayout;
		barrier.newLayout = _newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = _image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = 0;

		VkPipelineStageFlags srcStage{};
		VkPipelineStageFlags dstStage{};

		if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

			if (HasStencilComponent(_imageFormat))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			throw std::runtime_error("ERROR: Unsupported image layout transition");
		}

		vkCmdPipelineBarrier
		(
			cmdBuffer,
			srcStage, dstStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		EndSingleTimeCommands(_logicalDevice, _commandPool, _queue, cmdBuffer);
	}

	void VulkanUtils::CopyBufferToImage(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, const VkBuffer& _srcBuffer, const VkImage& _image, const uint32 _w, const uint32 _h) noexcept
	{
		VkCommandBuffer cmdBuffer = BeginSingleTimeCommands(_logicalDevice, _commandPool, _queue);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { _w, _h, 1 };

		vkCmdCopyBufferToImage
		(
			cmdBuffer,
			_srcBuffer,
			_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		EndSingleTimeCommands(_logicalDevice, _commandPool, _queue, cmdBuffer);
	}

	VkCommandBuffer VulkanUtils::BeginSingleTimeCommands(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue) noexcept
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer{};
		vkAllocateCommandBuffers(_logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

		return commandBuffer;
	}

	void VulkanUtils::EndSingleTimeCommands(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, const VkCommandBuffer& _commandBuffer) noexcept
	{
		vkEndCommandBuffer(_commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_commandBuffer;

		vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(_queue);

		vkFreeCommandBuffers(_logicalDevice, _commandPool, 1, &_commandBuffer);
	}
} // End of namespace