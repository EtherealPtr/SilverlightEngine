#include "Timer.h"
#include "Foundation/Logging/Logger.h"

namespace Silverlight
{
    Timer::Timer() noexcept :
        m_LastTime{ std::chrono::steady_clock::now() },
        m_StartTime{ std::chrono::steady_clock::now() },
        m_DeltaTime{ 0.0 },
        m_ElapsedTime{ 0.0 }
    {}

	void Timer::Update()
	{
        const auto currentTime{ std::chrono::steady_clock::now() };
        const std::chrono::duration<double> delta{ currentTime - m_LastTime };
        m_DeltaTime = delta.count();
        m_LastTime = currentTime;
	}

    void Timer::StartTimer() noexcept
    {
        m_StartTime = std::chrono::steady_clock::now();
    }

    void Timer::StopTimer()
    {
        const auto currentTime{ std::chrono::steady_clock::now() };
        const double frameTimeMs{ std::chrono::duration<double, std::milli>(currentTime - m_StartTime).count() };
        SE_LOG(LogCategory::Trace, "[Elapsed time]: %.3f ms", frameTimeMs);
    }
} // End of namespace