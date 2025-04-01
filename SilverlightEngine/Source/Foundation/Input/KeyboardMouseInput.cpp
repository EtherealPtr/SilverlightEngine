#include "KeyboardMouseInput.h"
#include "GLFW/glfw3.h"

namespace Silverlight
{
	KeyboardMouseInput::KeyboardMouseInput(GLFWwindow* _window) noexcept :
		m_Window{ _window },
		m_InputBindings{},
		m_MouseXChange{ 0.0 },
		m_MouseYChange{ 0.0 },
		m_LastMouseX{ 0.0 },
		m_LastMouseY{ 0.0 }
	{
		LockCursor();
	}

	void KeyboardMouseInput::Update()
	{
		UpdateKeyInputs();
		UpdateMousePosition();
	}

	void KeyboardMouseInput::UpdateKeyInputs()
	{
		for (const auto& binding : m_InputBindings)
		{
			if (IsKeyPressed(binding.m_Key))
			{
				binding.m_Callback();
			}
		}
	}

	void KeyboardMouseInput::UpdateMousePosition()
	{
		double mouseX{ 0.0 }, mouseY{ 0.0 };
		GetCursorPosition(mouseX, mouseY);

		m_MouseXChange = mouseX - m_LastMouseX;
		m_MouseYChange = mouseY - m_LastMouseY;

		m_LastMouseX = mouseX;
		m_LastMouseY = mouseY;
	}

	bool KeyboardMouseInput::IsKeyPressed(const int32 _key) const noexcept
	{
		return glfwGetKey(m_Window, _key) == GLFW_PRESS;
	}

	bool KeyboardMouseInput::IsButtonPressed(const int32 _button) const noexcept
	{
		return glfwGetMouseButton(m_Window, _button) == GLFW_PRESS;
	}

	void KeyboardMouseInput::GetCursorPosition(double& _x, double& _y) const noexcept
	{
		glfwGetCursorPos(m_Window, &_x, &_y);
	}

	void KeyboardMouseInput::LockCursor() noexcept
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
} // End of namespace