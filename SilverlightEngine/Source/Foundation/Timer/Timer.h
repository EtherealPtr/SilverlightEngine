#pragma once

#include "Foundation/Platform.h"
#include <chrono>

namespace Silverlight
{
	class Timer
	{
	public:
		Timer() noexcept;

		void Update();
		void StartTimer() noexcept;
		void StopTimer();
		double GetDeltaTime() const noexcept { return m_DeltaTime; }

		Timer(const Timer&) = delete;
		void operator=(const Timer&) = delete;
		Timer(Timer&&) = delete;
		void operator=(Timer&&) = delete;

	private:
		using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

		TimePoint m_LastTime;
		TimePoint m_StartTime;
		double m_DeltaTime;
		double m_ElapsedTime;
	};
} // End of namespace