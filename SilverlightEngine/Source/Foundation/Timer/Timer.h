#pragma once

#include "Foundation/Platform.h"
#include <chrono>
#include <string_view>

namespace Silverlight
{
	class Timer
	{
	public:
		Timer(std::string_view _funcName = "") noexcept;
		~Timer() noexcept;

		void Update();
		void ResetTimer() noexcept;
		double GetDeltaTime() const noexcept { return m_DeltaTime; }
		static double GetCurrentTimeMs() noexcept;

		Timer(const Timer&) = delete;
		void operator=(const Timer&) = delete;
		Timer(Timer&&) = delete;
		void operator=(Timer&&) = delete;

	private:
		using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

		std::string_view m_StringView;
		TimePoint m_LastTime;
		TimePoint m_StartTime;
		double m_DeltaTime;
		double m_ElapsedTime;
	};
} // End of namespace