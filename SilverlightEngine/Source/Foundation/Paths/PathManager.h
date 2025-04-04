#pragma once

#include <string>

namespace Silverlight
{
	class PathManager
	{
	public:
		static void InitializePaths();

		static const std::string& GetEngineAssetsPath() noexcept { return m_EngineAssetsDirPath; }
		static const std::string& GetUserAssetsPath() noexcept { return m_UserAssetsDirPath; }
		static const std::string& GetGeneratedDirPath() noexcept { return m_EngineGeneratedDirPath; }

		PathManager(const PathManager&) = delete;
		PathManager& operator=(const PathManager&) = delete;
		PathManager(PathManager&&) = delete;
		PathManager& operator=(PathManager&&) = delete;

	private:
		static std::string m_EngineAssetsDirPath;
		static std::string m_UserAssetsDirPath;
		static std::string m_EngineGeneratedDirPath;
	};
} // End of namespace