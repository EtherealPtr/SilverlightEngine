#include "Timer.h"
#include "Foundation/Logging/Logger.h"

namespace Silverlight
{
	Timer::Timer(std::string_view _funcName) noexcept :
		m_StringView{ _funcName },
		m_LastTime{ std::chrono::steady_clock::now() },
		m_StartTime{ std::chrono::steady_clock::now() },
		m_DeltaTime{ 0.0 },
		m_ElapsedTime{ 0.0 }
	{}

	Timer::~Timer() noexcept
	{
		const auto currentTime{ std::chrono::steady_clock::now() };
		const double frameTimeMs{ std::chrono::duration<double, std::milli>(currentTime - m_StartTime).count() };

		if (!m_StringView.empty())
		{
			SE_LOG(LogCategory::Profile, "Function {} took {:.2f} ms", m_StringView.data(), frameTimeMs);
		}
	}

	void Timer::Update()
	{
		const auto currentTime{ std::chrono::steady_clock::now() };
		const std::chrono::duration<double> delta{ currentTime - m_LastTime };
		m_DeltaTime = delta.count();
		m_LastTime = currentTime;
	}

	void Timer::ResetTimer() noexcept
	{
		m_StartTime = std::chrono::steady_clock::now();
	}
} // End of namespace