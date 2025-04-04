#include "PathManager.h"
#include <filesystem>

namespace Silverlight
{
	std::string PathManager::m_EngineAssetsDirPath{ "" };
	std::string PathManager::m_UserAssetsDirPath{ "" };
	std::string PathManager::m_EngineGeneratedDirPath{ "" };

	void PathManager::InitializePaths()
	{
		const std::filesystem::path currentFilePath(__FILE__);
		const std::filesystem::path currentFileDirectory{ currentFilePath.parent_path() };

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
				m_EngineAssetsDirPath = (dir / "build" / "Binaries").generic_string() + "/";
				m_UserAssetsDirPath = (dir / "Sandbox" / "Assets").generic_string() + "/";
				m_EngineGeneratedDirPath = (dir / "build" / "Silverlight_Generated").generic_string() + "/";
				return;
			}
		}
	}
} // End of namespace