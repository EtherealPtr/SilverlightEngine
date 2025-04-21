#pragma once

#include "Foundation/DLLConfig.h"
#include <string>

namespace Silverlight
{
	class PathManager
	{
	public:
		static void InitializePaths();

		SILVERLIGHT_ENGINE static const std::string& GetRuntimeAssetsPath() noexcept { return m_RuntimeAssetsDirPath; }
		SILVERLIGHT_ENGINE static const std::string& GetEngineAssetsPath() noexcept { return m_EngineAssetsDirPath; }
		SILVERLIGHT_ENGINE static const std::string& GetUserAssetsPath() noexcept { return m_UserAssetsDirPath; }
		SILVERLIGHT_ENGINE static const std::string& GetGeneratedDirPath() noexcept { return m_EngineGeneratedDirPath; }

		PathManager(const PathManager&) = delete;
		PathManager& operator=(const PathManager&) = delete;
		PathManager(PathManager&&) = delete;
		PathManager& operator=(PathManager&&) = delete;

	private:
		static std::string m_RuntimeAssetsDirPath;
		static std::string m_EngineAssetsDirPath;
		static std::string m_UserAssetsDirPath;
		static std::string m_EngineGeneratedDirPath;
	};
} // End of namespace