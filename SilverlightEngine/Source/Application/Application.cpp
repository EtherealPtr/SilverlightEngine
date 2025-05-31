#include "Application.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Foundation/Timer/Timer.h"
#include "Foundation/Entity/EntityManager.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"
#include <thread>

namespace Silverlight
{
	Application::Application() :
		m_Window{ nullptr },
		m_Renderer{ nullptr }
	{
		SE_LOG(LogCategory::Info, "[APPLICATION]: Initializing...");
		const EngineConfig configSettings{ INIParser::ParseConfigSettings("config.ini") };
		m_Window = std::make_unique<Window>(configSettings.m_WindowWidth, configSettings.m_WindowHeight, configSettings.m_WindowTitle);
	}

	Application::~Application()
	{
		SE_LOG(LogCategory::Info, "[APPLICATION]: Terminating");
	}

	void Application::PostClientInitialize()
	{
		try 
		{ 
			m_Renderer = std::make_unique<VulkanRenderer>(*m_Window.get()); 
		}
		catch (const std::exception& _problem)
		{
			SE_LOG(LogCategory::Error, "[APPLICATION]: Failed to initialize VulkanRenderer: {}", _problem.what());
		}
	}

	void Application::Run()
	{
		const double targetFrameTimeMs{ 16.67 };
		Timer timer{};
		SE_LOG(LogCategory::Info, "[APPLICATION]: Run");

		while (!m_Window->ShouldWindowClose())
		{
			double frameStart{ Timer::GetCurrentTimeMs() };
			
			timer.Update();

			m_Window->PollEvents();
			m_Renderer->GetCamera().ProcessInput(timer.GetDeltaTime());
			
			EntityManager::UpdateEntities(static_cast<float>(timer.GetDeltaTime()));

			m_Renderer->DrawFrame(timer.GetDeltaTime());

			ThrottleFrameRate(frameStart, targetFrameTimeMs);
		}
	}

	void Application::ThrottleFrameRate(double _frameStartTime, double _targetFrameTimeMs)
	{
		double frameEndTime{ Timer::GetCurrentTimeMs() };
		double elapsed{ frameEndTime - _frameStartTime };

		if (elapsed < _targetFrameTimeMs)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(_targetFrameTimeMs - elapsed)));
		}
	}
} // End of namespace