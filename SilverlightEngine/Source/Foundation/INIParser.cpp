#include "INIParser.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/Logging/Logger.h"

namespace Silverlight
{
	EngineConfig INIParser::ParseConfigSettings(std::string_view _filePath)
	{
		EngineConfig engineConfig{};

		std::ifstream file{ g_ResourceManager.ReadFile(_filePath.data()) };
		if (!file.is_open())
		{
			return engineConfig;
		}

		std::string line{ "" };

		while (std::getline(file, line))
		{
			// Skip comments or empty lines
			if (line.empty() || line.front() == '#' || line.front() == ';')
			{
				continue;
			}

			const size_t delimiterPos{ line.find('=') };
			if (delimiterPos == std::string_view::npos)
			{
				continue;
			}

			const std::string_view key{ std::string_view(line).substr(0, delimiterPos) };
			const std::string_view value{ std::string_view(line).substr(delimiterPos + 1) };

			if (key == "WindowTitle")
			{
				engineConfig.m_WindowTitle = value;
			}
			else if (key == "WindowWidth")
			{
				engineConfig.m_WindowWidth = std::stoul(std::string(value));
			}
			else if (key == "WindowHeight")
			{
				engineConfig.m_WindowHeight = std::stoul(std::string(value));
			}
		}

		SE_LOG(LogCategory::Info, "[CONFIG]: Loaded config.ini");
		return engineConfig;
	}
} // End of namespace