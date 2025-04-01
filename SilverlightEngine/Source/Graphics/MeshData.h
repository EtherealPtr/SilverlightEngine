#pragma once

#include "Material.h"

namespace Silverlight
{
	class MeshData
	{
	public:
		MeshData() noexcept :
			m_EntityId{ 0 },
			m_MeshId{ 0 },
			m_VertexBufferId{ 0 },
			m_IndexOffset{ 0 },
			m_IndexCount{ 0 },
			m_TexId{ 0 },
			m_Material{},
			m_Model{ 1.0f }
		{}
		~MeshData() = default;
		MeshData(const MeshData& _other) noexcept :
			m_EntityId(_other.m_EntityId),                
			m_MeshId(_other.m_MeshId),
			m_VertexBufferId(_other.m_VertexBufferId),    
			m_IndexOffset(_other.m_IndexOffset),          
			m_IndexCount(_other.m_IndexCount),
			m_TexId(_other.m_TexId),                      
			m_Material(_other.m_Material),                
			m_Model(_other.m_Model)
		{}
		MeshData& operator=(const MeshData& _other) noexcept
		{
			m_EntityId = _other.m_EntityId;
			m_MeshId = _other.m_MeshId;
			m_VertexBufferId = _other.m_VertexBufferId;
			m_IndexOffset = _other.m_IndexOffset;
			m_IndexCount = _other.m_IndexCount;
			m_TexId = _other.m_TexId;
			m_Material = _other.m_Material;
			m_Model = _other.m_Model;
			return *this;
		}
		MeshData(MeshData&& _other) noexcept : 
			m_EntityId(_other.m_EntityId),
			m_MeshId(_other.m_MeshId),
			m_VertexBufferId(_other.m_VertexBufferId),
			m_IndexOffset(_other.m_IndexOffset),
			m_IndexCount(_other.m_IndexCount),
			m_TexId(_other.m_TexId),
			m_Material(std::move(_other.m_Material)),
			m_Model(std::move(_other.m_Model))        
		{}
		MeshData& operator=(MeshData&& _other) noexcept
		{
			m_EntityId = _other.m_EntityId;
			m_MeshId = _other.m_MeshId;
			m_VertexBufferId = _other.m_VertexBufferId;
			m_IndexOffset = _other.m_IndexOffset;
			m_IndexCount = _other.m_IndexCount;
			m_TexId = _other.m_TexId;
			m_Material = std::move(_other.m_Material);
			m_Model = std::move(_other.m_Model);
			return *this;
		}

		void SetEntityId(const uint32 _entityId) noexcept { m_EntityId = _entityId; }
		void SetMeshId(const uint32 _meshId) noexcept { m_MeshId = _meshId; }
		void SetVertexBufferId(const uint32 _vertexBufferId) noexcept { m_VertexBufferId = _vertexBufferId; }
		void SetIndexOffset(const uint32 _indexOffset) noexcept { m_IndexOffset = _indexOffset; }
		void SetIndexCount(const uint32 _indices) noexcept { m_IndexCount = _indices; }
		void SetTexId(const uint32 _texId) noexcept { m_TexId = _texId; }
		void SetModelMatrix(const glm::mat4& _modelMatrix) noexcept { m_Model = _modelMatrix; }
		void SetDiffuseColor(const glm::vec4& _diffuseColor) noexcept { m_Material.SetDiffuseColor(_diffuseColor); }
		void SetSpecularColor(const glm::vec4& _specularColor) noexcept { m_Material.SetSpecularColor(_specularColor); }
		void SetShaderType(const ShaderTypeEnum _shaderType) noexcept { m_Material.SetShaderType(_shaderType); }

		uint32 GetEntityId() const noexcept { return m_EntityId; }
		uint32 GetMeshId() const noexcept { return m_MeshId; }
		uint32 GetVertexBufferId() const noexcept { return m_VertexBufferId; }
		uint32 GetIndexOffset() const noexcept { return m_IndexOffset; }
		uint32 GetIndexCount() const noexcept { return m_IndexCount; }
		uint32 GetTexId() const noexcept { return m_TexId; }
		const Material& GetMaterial() const noexcept { return m_Material; }
		const glm::mat4& GetModelMatrix() const noexcept { return m_Model; }

	private:
		uint32 m_EntityId;
		uint32 m_MeshId;
		uint32 m_VertexBufferId;
		uint32 m_IndexOffset;   // Offset into the index buffer
		uint32 m_IndexCount;
		uint32 m_TexId;
		Material m_Material;
		glm::mat4 m_Model;
	};
} // End of namespace