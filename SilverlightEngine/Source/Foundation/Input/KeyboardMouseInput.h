#pragma once

#include "Foundation/Input/InputDevice.h"
#include "Foundation/Input/InputBinding.h"
#include <vector>

struct GLFWwindow;

namespace Silverlight
{
	class KeyboardMouseInput : public InputDevice
	{
	public:
		KeyboardMouseInput(GLFWwindow* _window) noexcept;
		virtual void Update() override;
		virtual bool IsButtonPressed(const int32 _button) const noexcept override;
		virtual double GetXChange() const noexcept override { return m_MouseXChange; }
		virtual double GetYChange() const noexcept override { return m_MouseYChange; }
		bool IsKeyPressed(const int32 _key) const noexcept;
		void GetCursorPosition(double& _x, double& _y) const noexcept;
		void UpdateKeyInputs();
		void UpdateMousePosition();

	private:
		void LockCursor() noexcept;

	private:
		GLFWwindow* m_Window;
		std::vector<InputBinding> m_InputBindings;
		double m_MouseXChange;
		double m_MouseYChange;
		double m_LastMouseX;
		double m_LastMouseY;
	};
} // End of namespace