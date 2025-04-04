#pragma once

#include <string>

namespace Silverlight
{
	class PathManager
	{
	public:
		static void InitializePaths();

		static const std::string& GetEngineAssetsPath() noexcept { return m_EngineResDirPath; }
		static const std::string& GetUserAssetsPath() noexcept { return m_UserAssetsPath; }
		static const std::string& GetGeneratedDirPath() noexcept { return m_GeneratedDirPath; }

		PathManager(const PathManager&) = delete;
		PathManager& operator=(const PathManager&) = delete;
		PathManager(PathManager&&) = delete;
		PathManager& operator=(PathManager&&) = delete;

	private:
		static std::string m_EngineResDirPath;
		static std::string m_UserAssetsPath;
		static std::string m_GeneratedDirPath;
	};
} // End of namespace