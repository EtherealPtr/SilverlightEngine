#pragma once

#include "LogCategories.h"
#include <string>
#include <sstream>
#include <fstream>
#include <functional>

namespace Silverlight
{
	class Logger
	{
	public:
		Logger();
		~Logger();

		template<typename... Args>
		void PrintLog(const LogCategory _category, std::string_view _format, Args... _args) const;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(Logger&&) = delete;

	private:
		void OpenLogFile();
		void WriteToLogFile(const char* _logData);

	private:
		std::ofstream m_LogFile;
		std::function<void(const char*)> m_LogEvent;
	};

	extern const Logger g_Logger; // Defined in Logger.cpp

	template<typename... Args>
	inline void Logger::PrintLog(const LogCategory _category, std::string_view _format, Args... _args) const
	{
		std::ostringstream oss{};
		std::string_view categoryText{};

		constexpr std::string_view reset{ "\033[0m" };
		constexpr std::string_view red{ "\033[31m" };
		constexpr std::string_view green{ "\033[32m" };
		constexpr std::string_view yellow{ "\033[33m" };
		std::string_view color{ reset };
	
		switch (_category)
		{
		case LogCategory::Trace:
			categoryText = "[TRACE]";
			break;
		case LogCategory::Info:
			categoryText = "[INFO]";
			color = green;
			break;
		case LogCategory::Warning:
			categoryText = "[WARNING]";
			color = yellow;
			break;
		case LogCategory::Error:
			categoryText = "[ERROR]";
			color = red;
			break;
		}
	
		char buffer[1024]{};
		snprintf(buffer, sizeof(buffer), _format.data(), _args...);
		oss << categoryText << buffer;

		const std::string outputLog{ oss.str() };
		printf("%s%s\n", color.data(), outputLog.c_str());
		m_LogEvent(outputLog.c_str());
	}

#ifdef NDEBUG
#define SE_LOG(category, format, ...) ((void)0)
#else
#define SE_LOG(category, format, ...) g_Logger.PrintLog(category, format, __VA_ARGS__)
#endif
} // End of namespace