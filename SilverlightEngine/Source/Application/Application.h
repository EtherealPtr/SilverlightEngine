#pragma once

#include "Foundation/DLLConfig.h"
#include <memory>

namespace Silverlight
{
	class Window;
	class VulkanRenderer;

	class Application
	{
	public:
		SILVERLIGHT_ENGINE Application();
		SILVERLIGHT_ENGINE virtual ~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

		SILVERLIGHT_ENGINE void PostClientInitialize();
		SILVERLIGHT_ENGINE void Run() const;

	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<VulkanRenderer> m_Renderer;
	};

	std::unique_ptr<Application> CreateApplication(); // Defined in client project
} // End of namespace
