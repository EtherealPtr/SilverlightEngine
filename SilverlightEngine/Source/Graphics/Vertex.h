#pragma once

#include "glm/glm.hpp"

namespace Silverlight
{
	struct Vertex
	{
		Vertex(const float _pX = 0.0f, const float _pY = 0.0f, const float _pZ = 0.0f,
			   const float _u = 1.0f, const float _v = 1.0f,
			   const float _nX = 0.0f, const float _nY = 0.0f, const float _nZ = 0.0f) noexcept :
			m_Position{ _pX, _pY, _pZ },
			m_TexCoord{ _u, _v },
			m_Normal{ _nX, _nY, _nZ }
		{}

		glm::vec3 m_Position;
		glm::vec2 m_TexCoord;
		glm::vec3 m_Normal;
	};
} // End of namespace