#pragma once

#include "Foundation/Platform.h"
#include "Foundation/ResourceManager/File/FileManager.h"
#include "Foundation/ResourceManager/Image/ImageManager.h"
#include <string>
#include <fstream>
#include <vector>

namespace Silverlight
{
	class ResourceManager
	{
	public:
		ResourceManager() noexcept = default;
		~ResourceManager() = default;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		uint16 LoadImageResource(std::string_view _pathToImage) const;
		uint16 LoadImageFromMemory(const unsigned char* _bytes, const int32 _size) const;
		std::string GetAssetName(std::string_view _assetName) const;
		std::ifstream ReadFile(std::string_view _filePath) const;
		std::vector<char> ReadBinaryFile(std::string_view _fileName) const;
		const std::vector<Image>& GetImages() const noexcept;
		const ImageManager& GetImageManager() const noexcept { return m_ImageManager; }

	private:
		const FileManager m_FileManager;
		const ImageManager m_ImageManager;
	};

	extern const ResourceManager g_ResourceManager; // Defined in ResourceManager.cpp
} // End of namespace