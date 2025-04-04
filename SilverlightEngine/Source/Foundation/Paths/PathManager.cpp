#include "PathManager.h"
#include <filesystem>

namespace Silverlight
{
	std::string PathManager::m_EngineResDirPath{ "" };
	std::string PathManager::m_UserAssetsPath{ "" };
	std::string PathManager::m_GeneratedDirPath{ "" };

	void PathManager::InitializePaths()
	{
		const std::filesystem::path currentFilePath(__FILE__);
		const std::filesystem::path currentFileDirectory = currentFilePath.parent_path();

		for (std::filesystem::path dir = currentFileDirectory; !dir.empty(); dir = dir.parent_path())
		{
			bool foundSandbox{ false };
			bool foundEngine{ false };

			for (const auto& entry : std::filesystem::directory_iterator(dir))
			{
				if (!entry.is_directory())
				{
					continue;
				}

				if (entry.path().filename() == "Sandbox")
				{
					foundSandbox = true;
				}

				if (entry.path().filename() == "SilverlightEngine")
				{
					foundEngine = true;
				}
			}

			if (foundSandbox && foundEngine)
			{
				m_EngineResDirPath = (dir / "build" / "Binaries").generic_string() + "/";
				m_UserAssetsPath = (dir / "Sandbox" / "Assets").generic_string() + "/";
				m_GeneratedDirPath = (dir / "Silverlight_Generated").generic_string() + "/";
				return;
			}
		}
	}
} // End of namespace