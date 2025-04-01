#include "Timer.h"
#include "Foundation/Logging/Logger.h"

namespace Silverlight
{
    Timer::Timer() noexcept :
        m_LastTime{ clock_t::now() },
        m_StartTime{ clock_t::now() },
        m_DeltaTime{ 0.0 },
        m_ElapsedTime{ 0.0 },
        m_FrameCount{ 0 },
        m_Fps{ 0 },
        m_TimerRunning{ false },
        m_FpsDisplayEnabled{ false }
    {}

	void Timer::Update()
	{
        const auto currentTime{ clock_t::now() };
        const std::chrono::duration<double> delta{ currentTime - m_LastTime };
        m_DeltaTime = delta.count();
        m_LastTime = currentTime;

        if (!m_FpsDisplayEnabled)
        {
            return;
        }

        ++m_FrameCount;
        m_ElapsedTime += m_DeltaTime;

        if (m_ElapsedTime >= 1.0)
        {
            m_Fps = m_FrameCount;
            m_FrameCount = 0;
            m_ElapsedTime = 0.0;
            SE_LOG(LogCategory::Trace, "FPS: %d", m_Fps);
        }
	}

    void Timer::StartTimer() noexcept
    {
        m_TimerRunning = true;
        m_StartTime = clock_t::now();
    }

    void Timer::StopTimer()
    {
        if (!m_TimerRunning)
		{
			return;
		}

        const auto elapsedTime{ std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_StartTime).count() };
        SE_LOG(LogCategory::Trace, "[Elapsed time]: %d", elapsedTime);
        m_TimerRunning = false;
    }
} // End of namespace