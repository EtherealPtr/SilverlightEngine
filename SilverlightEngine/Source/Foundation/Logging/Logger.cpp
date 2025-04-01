#include "Logger.h"
#include "Foundation/Paths/PathManager.h"

namespace Silverlight
{
	const Logger g_Logger{};

	Logger::Logger() :
		m_LogFile{},
		m_LogEvent{}
	{
		m_LogEvent = [this](const char* _logData) { WriteToLogFile(_logData); };
	}

	Logger::~Logger()
	{
		if (m_LogFile.is_open())
		{
			m_LogFile.close();
		}
	}

	void Logger::OpenLogFile()
	{
		if (m_LogFile.is_open())
		{
			return;
		}

		m_LogFile.open(PathManager::GetGeneratedDirPath() + "logs.txt", std::ios::out | std::ios::trunc);
		
		if (!m_LogFile.is_open())
		{
			throw std::runtime_error("ERROR: Failed to open log file.");
		}
	}

	void Logger::WriteToLogFile(const char* _logData)
	{
		OpenLogFile();
		m_LogFile << _logData << '\n';
	}
} // End of namespace