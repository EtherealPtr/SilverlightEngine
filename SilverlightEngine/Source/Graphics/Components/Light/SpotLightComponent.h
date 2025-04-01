#pragma once

#include "LightComponent.h"

namespace Silverlight
{
	class SpotLightComponent : public LightComponent
	{
	public:
		SILVERLIGHT_ENGINE SpotLightComponent(const glm::vec3& _direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec4& _color = glm::vec4(1.0f), const float _radius = 10.0f, const float _innerCutoffAngle = 12.5f, const float _outerCutoffAngle = 17.5f);
		
		virtual void UpdatePosition() noexcept override;
		void SetPosition(const glm::vec3& _pos) noexcept;

	private:
		void UpdateAttenuation(const float _radius) noexcept;
		void UpdateCutoffAngles(const float _innerCutoffAngle, const float _outerCutoffAngle) noexcept;
	};
} // End of namespace