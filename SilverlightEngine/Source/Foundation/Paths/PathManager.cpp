#include "PathManager.h"
#include <filesystem>

namespace Silverlight
{
	std::string PathManager::m_EngineResDirPath{ "" };
	std::string PathManager::m_GeneratedDirPath{ "" };

	void PathManager::InitializePaths()
	{
		const std::filesystem::path currentFilePath(__FILE__);
		const std::filesystem::path currentFileDirectory = currentFilePath.parent_path();

		// Traverse the directory tree upwards until we find the "Res" folder
		for (std::filesystem::path directory = currentFileDirectory; !directory.empty(); directory = directory.parent_path())
		{
			for (const auto& entry : std::filesystem::directory_iterator(directory))
			{
				if (entry.is_directory() && entry.path().filename() == "Res")
				{
					m_GeneratedDirPath = entry.path().parent_path().generic_string() + "/Silverlight_Generated/";
					m_EngineResDirPath = entry.path().generic_string() + '/';
					return;
				}
			}
		}
	}
} // End of namespace