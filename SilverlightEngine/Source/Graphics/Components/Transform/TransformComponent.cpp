#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Silverlight
{
	void TransformComponent::SetPosition(const glm::vec3& _position) noexcept
	{
		m_Position = _position;
	}

	void TransformComponent::SetRotation(const glm::quat& _rotation) noexcept
	{
		m_Rotation = _rotation;
	}

	void TransformComponent::SetScale(const glm::vec3& _scale) noexcept
	{
		m_Scale = _scale;
	}

	void TransformComponent::Translate(const glm::vec3& _translation) noexcept
	{
		m_Position += _translation;
	}

	void TransformComponent::Rotate(const glm::quat& _rotation) noexcept
	{
		m_Rotation *= _rotation;
	}

	void TransformComponent::Scale(const glm::vec3& _scale) noexcept
	{
		m_Scale *= _scale;
	}

	glm::mat4 TransformComponent::GetModel() const
	{
		const glm::mat4 translationMatrix{ glm::translate(glm::mat4(1.0f), m_Position) };
		const glm::mat4 rotationMatrix{ glm::mat4_cast(m_Rotation) };
		const glm::mat4 scaleMatrix{ glm::scale(glm::mat4(1.0f), m_Scale) };
		return translationMatrix * rotationMatrix * scaleMatrix;
	}
} // End of namespace