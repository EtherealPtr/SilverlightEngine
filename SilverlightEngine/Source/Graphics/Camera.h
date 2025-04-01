#pragma once

#include "Foundation/Input/KeyboardMouseInput.h"
#include "DataStructs.h"
#include <memory>

struct GLFWwindow;

namespace Silverlight
{
	class Camera
	{
	public:
		Camera(const float _fov, const float _aspect, const float _near, const float _far, GLFWwindow* _wnd = nullptr);

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;

		const ViewProjMatrix& GetViewProjMatrix() const noexcept { return m_ViewProjMatrix; }
		const glm::mat4& GetViewMatrix() const noexcept { return m_ViewProjMatrix.m_View; }
		const glm::mat4& GetProjectionMatrix() const noexcept { return m_ViewProjMatrix.m_Proj; }
		const glm::vec3& GetPosition() const noexcept { return m_Position; }
		void ProcessInput(const double _deltaTime);

	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void ProcessKeyboardInput(const double _deltaTime);
		void ProcessMouseInput(const double _deltaTime);

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_Front;
		float m_Fov;
		float m_Aspect;
		float m_Near;
		float m_Far;
		float m_Yaw;
		float m_Pitch;
		ViewProjMatrix m_ViewProjMatrix;
		KeyboardMouseInput m_InputDevice;
	};
} // End of namespace