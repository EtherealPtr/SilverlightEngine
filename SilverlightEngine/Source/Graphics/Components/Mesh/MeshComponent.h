#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/DLLConfig.h"
#include "Graphics/ShaderTypeEnum.h"
#include <glm/glm.hpp>
#include <optional>

namespace Silverlight
{
	class MeshComponent : public Component
	{
	public:
		SILVERLIGHT_ENGINE MeshComponent() noexcept;
		SILVERLIGHT_ENGINE ~MeshComponent() = default;

		virtual void SetTintColor(const glm::vec4& _tintColor) noexcept { m_TintColor = _tintColor; }
		virtual void SetVertexBufferId(const uint32 _vertexBufferId) noexcept { m_VertexBufferId = _vertexBufferId; }
		uint32 GetVertexBufferId() const noexcept { return m_VertexBufferId; }
		ShaderTypeEnum GetShaderType() const noexcept { return m_ShaderType; }
		const std::optional<glm::vec4>& GetTintColor() const noexcept { return m_TintColor; }

	protected:
		uint32 m_VertexBufferId;
		ShaderTypeEnum m_ShaderType;
		std::optional<glm::vec4> m_TintColor;
	};
} // End of namespace