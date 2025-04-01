#include "PointLightComponent.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Transform/TransformComponent.h"

namespace Silverlight
{
	PointLightComponent::PointLightComponent(const glm::vec4& _color, const float _radius)
	{
		m_LightData.m_Color = _color;

		UpdateAttenuation(_radius);
	}

	void PointLightComponent::UpdatePosition() noexcept
	{
		const auto transform{ GetOwner()->GetComponent<TransformComponent>() };
		SetPosition(transform->GetPosition());
	}

	void PointLightComponent::SetPosition(const glm::vec3& _pos)
	{
		m_LightData.m_PositionAndType = glm::vec4(_pos, static_cast<float>(LightType::Point));
	}

	void PointLightComponent::UpdateAttenuation(const float _radius)
	{
		constexpr float constant{ 1.0f };
		const float linear{ 4.5f / _radius };
		const float quadratic{ 75.0f / (_radius * _radius) };

		m_LightData.m_Attenuation = glm::vec4(constant, linear, quadratic, 0.0f);
	}
} // End of namespace