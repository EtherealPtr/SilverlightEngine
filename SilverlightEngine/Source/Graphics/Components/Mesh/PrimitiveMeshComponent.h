#pragma once

#include "MeshComponent.h"
#include "Graphics/PrimitiveShapeEnum.h"
#include "Graphics/MeshData.h"
#include <string>

namespace Silverlight
{
	class PrimitiveMeshComponent : public MeshComponent
	{
	public:
		SILVERLIGHT_ENGINE PrimitiveMeshComponent(const PrimitiveShapeEnum _basicShape) noexcept;

		virtual void OnComponentInitialized() noexcept override;
		virtual void SetTintColor(const glm::vec4& _tintColor) noexcept override;
		virtual void SetVertexBufferId(const uint32 _vertexBufferId) noexcept override;
		SILVERLIGHT_ENGINE void SetTexture(std::string_view _texturePath);
		void SetIndices(const uint32 _indices) noexcept { m_Data.SetIndexCount(_indices); }
		PrimitiveShapeEnum GetShape() const noexcept { return m_Shape; }
		MeshData& GetMeshData() noexcept { return m_Data; }

	private:
		PrimitiveShapeEnum m_Shape;
		MeshData m_Data;
	};
} // End of namespace