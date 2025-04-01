#pragma once

typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkSampler_T* VkSampler;

namespace Silverlight
{
	class VulkanDevice;

	class VulkanTextureSampler
	{
	public:
		VulkanTextureSampler(const VulkanDevice& _device);
		~VulkanTextureSampler();

		VulkanTextureSampler(const VulkanTextureSampler&) = delete;
		VulkanTextureSampler& operator=(const VulkanTextureSampler&) = delete;
		VulkanTextureSampler(VulkanTextureSampler&&) = delete;
		VulkanTextureSampler& operator=(VulkanTextureSampler&&) = delete;

		const VkSampler& Get() const noexcept { return m_TextureSampler; }

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkSampler m_TextureSampler;
	};
} // End of namespace