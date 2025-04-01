#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Components/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Silverlight
{
	class TransformComponent : public Component
	{
	public:
		SILVERLIGHT_ENGINE TransformComponent() noexcept :
			m_Position{ glm::vec3(0.0f, 0.0f, 0.0f) },
			m_Scale{ glm::vec3(1.0f) },
			m_Rotation{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f) }
		{}

		SILVERLIGHT_ENGINE void SetPosition(const glm::vec3& _position) noexcept;
		SILVERLIGHT_ENGINE void SetRotation(const glm::quat& _rotation) noexcept;
		SILVERLIGHT_ENGINE void SetScale(const glm::vec3& _scale) noexcept;
		SILVERLIGHT_ENGINE void Translate(const glm::vec3& _translation) noexcept;
		SILVERLIGHT_ENGINE void Rotate(const glm::quat& _rotation) noexcept;
		SILVERLIGHT_ENGINE void Scale(const glm::vec3& _scale) noexcept;
		SILVERLIGHT_ENGINE glm::mat4 GetModel() const;
		SILVERLIGHT_ENGINE const glm::vec3& GetPosition() const noexcept { return m_Position; }
		SILVERLIGHT_ENGINE const glm::vec3& GetScale() const noexcept { return m_Scale; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;
	};
} // End of namespace