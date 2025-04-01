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
		void ToggleFpsDisplay(const bool _toggle) noexcept { m_FpsDisplayEnabled = _toggle; }
		double GetDeltaTime() const noexcept { return m_DeltaTime; }
		void StartTimer() noexcept;
		void StopTimer();

		Timer(const Timer&) = delete;
		Timer(Timer&&) = delete;
		void operator=(const Timer&) = delete;
		void operator=(Timer&&) = delete;

	private:
		using clock_t = std::chrono::steady_clock;
		using time_point_t = std::chrono::time_point<clock_t>;

		time_point_t m_LastTime;
		time_point_t m_StartTime;
		double m_DeltaTime;
		double m_ElapsedTime;
		int32 m_FrameCount;
		int32 m_Fps;
		bool m_TimerRunning;
		bool m_FpsDisplayEnabled;
	};
} // End of namespace