#pragma once

#include <vector>
#include <fstream>

namespace Silverlight
{
	class FileManager
	{
	public:
		FileManager();

		FileManager(const FileManager&) = delete;
		void operator=(const FileManager&) = delete;
		FileManager(FileManager&&) = delete;
		void operator=(FileManager&&) = delete;

		std::vector<char> ReadBinaryFile(std::string_view _fileName) const;
		std::ifstream ReadFile(std::string_view _filePath) const;
		std::string GetAssetName(std::string_view _assetPath) const;
	};
} // End of namespace