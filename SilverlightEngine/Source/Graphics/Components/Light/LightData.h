#pragma once

#include "Foundation/Platform.h"
#include <glm/glm.hpp>

namespace Silverlight
{
	enum class LightType : uint8
	{
		Directional,
		Point,
		Spotlight
	};

	struct LightData
	{
		LightData() noexcept :
			m_PositionAndType{ glm::vec4(0.0f) },
			m_Direction{ glm::vec4(0.0f) },
			m_Color{ glm::vec4(0.0f) },
			m_Attenuation{ glm::vec4(0.0f) },
			m_CutoffAngles{ glm::vec4(0.0f) }
		{}

		glm::vec4 m_PositionAndType;	// Last element is type
		glm::vec4 m_Direction;			// Last element is padding
		glm::vec4 m_Color;				// Last element is color intensity
		glm::vec4 m_Attenuation;		// Last element is padding
		glm::vec4 m_CutoffAngles;		// First element is inner cutoff angle, last element is outer cutoff angle
	};
} // End of namespace