#pragma once

#include "Foundation/Platform.h"
#include "Components/Light/LightData.h"
#include <glm/glm.hpp>

namespace Silverlight
{
	struct ViewProjMatrix
	{
		ViewProjMatrix() noexcept = default;
		ViewProjMatrix(const glm::mat4& _view, const glm::mat4& _proj) noexcept :
			m_View{ _view },
			m_Proj{ _proj }
		{}

		glm::mat4 m_View{ glm::mat4(1.0f) };
		glm::mat4 m_Proj{ glm::mat4(1.0f) };
	};

	struct PushConstant
	{
		PushConstant(const glm::mat4& _model, const glm::vec3& _camPos, const uint32 _texId) noexcept :
			m_Model{ _model },
			m_CameraPos{ _camPos },
			m_TextureIndex{ _texId }
		{}

		const glm::mat4 m_Model{ glm::mat4(1.0f) };
		const glm::vec3 m_CameraPos{ glm::vec3(0.0f) };
		const uint32 m_TextureIndex{ 0 };
	};

	struct LightBuffer
	{
		alignas(16) uint32 m_TotalLights { 0 };
		alignas(16) LightData m_Lights[25]{};
	};
} // End of namespace