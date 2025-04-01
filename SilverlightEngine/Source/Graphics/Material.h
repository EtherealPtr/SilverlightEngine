#pragma once

#include "Foundation/Platform.h"
#include "Graphics/ShaderTypeEnum.h"
#include <glm/glm.hpp>

namespace Silverlight
{
	class Material
	{
	public:
		Material(const glm::vec3& _diffuse = glm::vec3(1.0f), const glm::vec3& _specular = glm::vec3(1.0f), float _shininess = 8.0f, const ShaderTypeEnum _shaderType = ShaderTypeEnum::Standard) noexcept :
			m_DiffuseColor{ glm::vec4{_diffuse, 1.0f} },
			m_SpecularColor{ glm::vec4(_specular, _shininess) },
			m_ShaderType{ _shaderType }
		{}

		void SetDiffuseColor(const glm::vec4& _diffuseColor) noexcept { m_DiffuseColor = _diffuseColor; }
		void SetSpecularColor(const glm::vec4& _specularColor) noexcept { m_SpecularColor = _specularColor; }
		void SetShaderType(const ShaderTypeEnum _shaderType) noexcept { m_ShaderType = _shaderType; }
		const glm::vec4& GetDiffuseColor() const noexcept { return m_DiffuseColor; }
		const glm::vec4& GetSpecularColor() const noexcept { return m_SpecularColor; }
		ShaderTypeEnum GetShaderType() const noexcept { return m_ShaderType; }

	private:
		glm::vec4 m_DiffuseColor;
		glm::vec4 m_SpecularColor;
		ShaderTypeEnum m_ShaderType;
	};
} // End of namespace
