#pragma once

#include "Foundation/Platform.h"
#include "GLMConfig.h"

namespace Silverlight
{
	class Material
	{
	public:
		Material(const glm::vec3& _diffuse = glm::vec3(1.0f), const glm::vec3& _specular = glm::vec3(1.0f), float _shininess = 8.0f) noexcept :
			m_DiffuseColor{ glm::vec4{_diffuse, 1.0f} },
			m_SpecularColor{ glm::vec4(_specular, _shininess) }
		{}

		void SetDiffuseColor(const glm::vec4& _diffuseColor) noexcept { m_DiffuseColor = _diffuseColor; }
		void SetSpecularColor(const glm::vec4& _specularColor) noexcept { m_SpecularColor = _specularColor; }
		const glm::vec4& GetDiffuseColor() const noexcept { return m_DiffuseColor; }
		const glm::vec4& GetSpecularColor() const noexcept { return m_SpecularColor; }

	private:
		glm::vec4 m_DiffuseColor;
		glm::vec4 m_SpecularColor;
	};
} // End of namespace
