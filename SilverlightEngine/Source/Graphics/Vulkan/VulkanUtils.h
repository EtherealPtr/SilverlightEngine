#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;
typedef struct VkShaderModule_T* VkShaderModule;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkQueue_T* VkQueue;
typedef enum VkFormat VkFormat;
typedef enum VkImageTiling VkImageTiling;
typedef enum VkImageUsageFlagBits VkImageUsageFlagBits;
typedef enum VkImageLayout VkImageLayout;

namespace Silverlight
{
	class VulkanUtils
	{
	public:
		static void CheckInstanceExtSupport(const std::vector<const char*>& _requiredExtensions);
		static void CheckInstanceLayerSupport(const std::vector<const char*>& _requiredLayers);
		static void CheckDeviceExtSupport(const VkPhysicalDevice& _gpu, const std::vector<const char*>& _requiredExtensions);
		static VkShaderModule CreateShaderModule(const VkDevice& _logicalDevice, const std::vector<char>& _shaderBinaryCode);
		static void CreateBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint64 _size, const uint32 _usage, const uint32 _memoryPropertyFlags, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory);
		static void CreateImage(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32 _w, const uint32 _h, const VkFormat _format, const VkImageTiling _tiling, const VkImageUsageFlagBits _usage, const uint32 _memoryPropertyFlags, VkImage& _image, VkDeviceMemory& _imageMemory);
		static void CreateImageView(const VkDevice& _logicalDevice, const VkImage& _image, const uint32 _format, const uint32 _aspect, VkImageView& _imageView);
		static uint32 FindMemoryTypeIndex(const VkPhysicalDevice& _gpu, const uint32 _memoryTypeBits, const uint32 _memoryPropertyFlags) noexcept;
		static void CopyBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, const uint64 _size, const VkBuffer& _srcBuffer, const VkBuffer& _dstBuffer);
		static VkFormat FindSupportedFormat(const VkPhysicalDevice& _gpu, const std::vector<VkFormat>& _formats, const VkImageTiling _tiling, const uint32 _formatFeatures);
		static constexpr bool HasStencilComponent(const VkFormat _format) noexcept;
		static void TransitionImageLayout(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, VkImage& _image, const VkFormat _imageFormat, const VkImageLayout _oldLayout, const VkImageLayout _newLayout);
		static void CopyBufferToImage(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, const VkBuffer& _srcBuffer, const VkImage& _image, const uint32 _w, const uint32 _h) noexcept;
	
	private:
		static VkCommandBuffer BeginSingleTimeCommands(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue) noexcept;
		static void EndSingleTimeCommands(const VkDevice& _logicalDevice, const VkCommandPool& _commandPool, const VkQueue& _queue, const VkCommandBuffer& _commandBuffer) noexcept;
	};
} // End of namespace