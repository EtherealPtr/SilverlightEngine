#pragma once

#include "Foundation/Platform.h"
#include <string_view>

struct GLFWwindow;

namespace Silverlight
{
	class Window
	{
	public:
		Window(const uint16 _width, const uint16 _height, const std::string_view _title);
		~Window();

		Window(const Window&) = delete;
		void operator=(const Window&) = delete;
		Window(Window&&) = delete;
		void operator=(Window&&) = delete;

		bool ShouldWindowClose() const noexcept;
		void PollEvents() noexcept;
		GLFWwindow* GetWindow() const noexcept { return m_Window; }
		int GetFramebufferWidth() const noexcept;
		int GetFramebufferHeight() const noexcept;

	private:
		GLFWwindow* m_Window;
	};
} // End of namespace