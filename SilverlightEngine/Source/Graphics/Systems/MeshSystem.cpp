#include "MeshSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Vulkan/VulkanVertexBufferManager.h"
#include "Graphics/Components/Mesh/PrimitiveMeshComponent.h"
#include "Graphics/Components/Mesh/CustomMeshComponent.h"
#include <algorithm>

namespace Silverlight
{
	MeshSystem::MeshSystem(VulkanVertexBufferManager& _vertexBufferManager) :
		m_VertexBufferManager{ _vertexBufferManager },
		m_CachedSubMeshes{},
		m_VertexBufferIdToSubMeshes{},
		m_TotalMeshCount{ 0 }
	{}

	void MeshSystem::ProcessComponents(const Entity* const _entity)
	{
		if (const auto& primitiveMesh{ _entity->GetComponent<PrimitiveMeshComponent>() })
		{
			m_VertexBufferManager.CreateBasicShapeVertexBuffer(*primitiveMesh.get());
			AddMesh(primitiveMesh.get()->GetMeshData());
		}
		else if (const auto& modelMesh{ _entity->GetComponent<CustomMeshComponent>() })
		{
			m_VertexBufferManager.CreateModelVertexBuffer(*modelMesh.get());
			const auto& existingSubMeshes{ GetSubMeshes(modelMesh.get()->GetVertexBufferId()) };

			if (existingSubMeshes.empty())
			{
				AddMeshes(modelMesh.get()->GetMeshData());
				return;
			}

			std::vector<MeshData> duplicatedMeshes(existingSubMeshes.size());
			std::transform(existingSubMeshes.begin(), existingSubMeshes.end(), duplicatedMeshes.begin(), [&](const MeshData& _subMesh)
				{
					MeshData copiedSubMesh{ _subMesh };
					copiedSubMesh.SetEntityId(modelMesh.get()->GetOwner()->GetUniqueId());
					copiedSubMesh.SetShaderType(modelMesh.get()->GetShaderType());

					if (modelMesh.get()->GetTintColor().has_value())
					{
						const glm::vec4& tintColor{ modelMesh.get()->GetTintColor().value() };
						copiedSubMesh.SetDiffuseColor(tintColor);
					}

					return copiedSubMesh;
				});

			AddMeshes(duplicatedMeshes);
		}
	}

	void MeshSystem::AddMeshes(std::vector<MeshData>& _meshes)
	{
		for (auto& mesh : _meshes)
		{
			AddMesh(mesh);
		}

		UpdateSubMeshCache();
	}

	void MeshSystem::AddMesh(MeshData& _mesh)
	{
		const uint32 vertexBufferId{ _mesh.GetVertexBufferId() };
		_mesh.SetMeshId(m_TotalMeshCount++);
		m_VertexBufferIdToSubMeshes[vertexBufferId].emplace_back(_mesh);
		UpdateSubMeshCache();
	}

	const std::vector<MeshData>& MeshSystem::GetSubMeshes(const uint32 _bufferId) const
	{
		static const std::vector<MeshData> empty{};

		const auto it{ m_VertexBufferIdToSubMeshes.find(_bufferId) };
		if (it != m_VertexBufferIdToSubMeshes.end())
		{
			return it->second;
		}

		return empty;
	}

	VulkanVertexBuffer* MeshSystem::GetVertexBuffer(const uint32 _bufferId)
	{
		return m_VertexBufferManager.GetVertexBuffer(_bufferId);
	}

	void MeshSystem::UpdateSubMeshCache()
	{
		m_CachedSubMeshes.clear();

		for (const auto& mesh : m_VertexBufferIdToSubMeshes)
		{
			const auto& subMeshes{ mesh.second };
			m_CachedSubMeshes.insert(m_CachedSubMeshes.end(), subMeshes.begin(), subMeshes.end());
		}
	}
} // End of namespace