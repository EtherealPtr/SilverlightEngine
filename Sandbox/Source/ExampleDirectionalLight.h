#pragma once

#include "Foundation/Entity/Entity.h"
#include "Components.h"

namespace ExampleApp
{
	class ExampleDirectionalLight : public Silverlight::Entity
	{
	public:
		ExampleDirectionalLight(const glm::vec3& _direction = glm::vec3(0.5f, -0.2f, -1.0f), const glm::vec4& _color = glm::vec4(1.0f))
		{
			m_Transform = AddComponent<Silverlight::TransformComponent>();
			AddComponent<Silverlight::DirectionalLightComponent>(_direction, _color);
		}

	private:
		std::shared_ptr<Silverlight::TransformComponent> m_Transform;
	};
} // End of namespace