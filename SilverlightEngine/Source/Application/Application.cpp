#include "Application.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/INIParser.h"
#include "Foundation/Timer/Timer.h"
#include "Foundation/Entity/EntityManager.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan/VulkanRenderer.h"

namespace Silverlight
{
	Application::Application() :
		m_Window{ nullptr },
		m_Renderer{ nullptr }
	{
		SE_LOG(LogCategory::Trace, "[APPLICATION]: Initializing...");
		const EngineConfig configSettings{ INIParser::ParseConfigSettings("config.ini") };
		m_Window = std::make_unique<Window>(configSettings.m_WindowWidth, configSettings.m_WindowHeight, configSettings.m_WindowTitle);
	}

	Application::~Application()
	{
		SE_LOG(LogCategory::Trace, "[APPLICATION]: Terminating");
	}

	void Application::PostClientInitialize()
	{
		try 
		{ 
			m_Renderer = std::make_unique<VulkanRenderer>(*m_Window.get()); 
		}
		catch (const std::exception& _problem)
		{
			SE_LOG(LogCategory::Error, "[APPLICATION]: Failed to initialize VulkanRenderer: %s", _problem.what());
		}
	}

	void Application::Run() const
	{
		Timer timer{};
		SE_LOG(LogCategory::Trace, "[APPLICATION]: Run");

		while (!m_Window->ShouldWindowClose())
		{
			timer.Update();
			m_Window->PollEvents();
			EntityManager::UpdateEntities(static_cast<float>(timer.GetDeltaTime()));
			m_Renderer->DrawFrame(timer.GetDeltaTime());
		}
	}
} // End of namespace