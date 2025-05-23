#pragma once

#include "LogCategories.h"
#include <string>
#include <fstream>
#include <functional>
#include <format>

namespace Silverlight
{
	class Logger
	{
	public:
		Logger();
		~Logger();

		template<typename... Args>
		void PrintLog(const LogCategory _category, std::format_string<Args...> _format, Args&&... _args) const;

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
	inline void Logger::PrintLog(const LogCategory _category, std::format_string<Args...> _format, Args&&... _args) const
	{
		std::string_view categoryText{};
		constexpr std::string_view reset{ "\033[0m" };
		constexpr std::string_view green{ "\033[32m" };
		constexpr std::string_view magenta{ "\033[35m" };
		constexpr std::string_view yellow{ "\033[33m" };
		constexpr std::string_view red{ "\033[31m" };
		std::string_view color{ reset };

		switch (_category)
		{
		case LogCategory::Info:
			categoryText = "[INFO]";
			color = green;
			break;
		case LogCategory::Profile:
			categoryText = "[PROFILER]";
			color = magenta;
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

		auto formatted{ std::format(_format, std::forward<Args>(_args)...) };
		auto outputLog{ std::format("{}{}", categoryText, formatted) };
		std::printf("%s%s%s\n", color.data(), outputLog.c_str(), reset.data());
		m_LogEvent(outputLog.c_str());
	}

#ifdef NDEBUG
#define SE_LOG(category, format, ...) ((void)0)
#else
#define SE_LOG(category, format, ...) g_Logger.PrintLog(category, format, __VA_ARGS__)
#endif
} // End of namespace