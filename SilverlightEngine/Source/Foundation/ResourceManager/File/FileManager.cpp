#include "FileManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/DLLConfig.h"
#include "Foundation/Paths/PathManager.h"
#include <filesystem>

namespace Silverlight
{
	FileManager::FileManager() 
	{
		PathManager::InitializePaths();

		if (!std::filesystem::exists(PathManager::GetGeneratedDirPath()))
		{
			std::filesystem::create_directory(PathManager::GetGeneratedDirPath());
		}
	}

	std::vector<char> FileManager::ReadBinaryFile(std::string_view _fileName) const
	{
		std::ifstream inputFile(PathManager::GetEngineResDirPath() + _fileName.data(), std::ios::binary | std::ios::ate);
		if (!inputFile.is_open())
		{
			SE_LOG(LogCategory::Warning, "[FILEMANAGER]: Failed to read binary file: %s", _fileName.data());
		}

		const size_t fileSize = static_cast<size_t>(inputFile.tellg());
		std::vector<char> buffer(fileSize);

		inputFile.seekg(0);
		inputFile.read(buffer.data(), fileSize);
		inputFile.close();

		return buffer;
	}
	
	std::ifstream FileManager::ReadFile(std::string_view _filePath) const
	{
		const std::string fullPath = PathManager::GetEngineResDirPath() + _filePath.data();
		std::ifstream file(fullPath);

		if (!file.is_open())
		{
			SE_LOG(LogCategory::Error, "[FILEMANAGER]: Failed to open file: %s", _filePath.data());
		}

		return file;
	}

	std::string FileManager::GetAssetName(const std::string_view _assetPath) const
	{
		std::filesystem::path path(_assetPath);
		return std::move(path.filename().string());
	}
} // End of namespace