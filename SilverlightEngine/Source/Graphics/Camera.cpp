#include "Camera.h"
#include "Foundation/Input/InputMapping.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace Silverlight
{
	Camera::Camera(const float _fov, const float _aspect, const float _near, const float _far, GLFWwindow* _wnd) :
		m_Position{ glm::vec3(0.0f, 5.0f, 7.0f) },
		m_Up{ glm::vec3(0.0f, 1.0f, 0.0f) },
		m_Right{ glm::vec3(1.0f, 0.0f, 0.0f) },
		m_Front{ glm::vec3(0.0f, 0.0f, -1.0f) },
		m_Fov{ _fov },
		m_Aspect{ _aspect },
		m_Near{ _near },
		m_Far{ _far },
		m_Yaw{ -90.0f },
		m_Pitch{ 0.0f },
		m_ViewProjMatrix{},
		m_InputDevice{ _wnd }
	{
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::ProcessInput(const double _deltaTime)
	{
		ProcessKeyboardInput(_deltaTime);
		ProcessMouseInput(_deltaTime);
		m_InputDevice.UpdateMousePosition();
	}

	void Camera::ProcessKeyboardInput(const double _deltaTime)
	{
		constexpr float baseSpeed{ 15.0f };
		const float speed{ baseSpeed * static_cast<float>(_deltaTime) };

		if (m_InputDevice.IsKeyPressed(SE_KEY_W))
		{
			m_Position += speed * m_Front;
		}
		if (m_InputDevice.IsKeyPressed(SE_KEY_S))
		{
			m_Position -= speed * m_Front;
		}
		if (m_InputDevice.IsKeyPressed(SE_KEY_A))
		{
			m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * speed;
		}
		if (m_InputDevice.IsKeyPressed(SE_KEY_D))
		{
			m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * speed;
		}

		UpdateViewMatrix();
	}

	void Camera::ProcessMouseInput(const double _deltaTime)
	{
		double xoffset{ m_InputDevice.GetXChange() };
		double yoffset{ m_InputDevice.GetYChange() };

		constexpr double baseSensitivity{ 25.0 };
		const double sensitivity{ baseSensitivity * _deltaTime };
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw += static_cast<float>(xoffset);
		m_Pitch -= static_cast<float>(yoffset);
		m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		glm::vec3 front{};
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		m_ViewProjMatrix.m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void Camera::UpdateProjectionMatrix()
	{
		m_ViewProjMatrix.m_Proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
	}
} // End of namespace