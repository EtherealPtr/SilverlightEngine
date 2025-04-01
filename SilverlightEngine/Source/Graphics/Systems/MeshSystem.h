#pragma once

#include "Foundation/Platform.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace Silverlight
{
	class VulkanVertexBufferManager;
	class VulkanVertexBuffer;
	class MeshData;
	class Entity;

	class MeshSystem
	{
	public:
		MeshSystem(VulkanVertexBufferManager& _vertexBufferManager);
		~MeshSystem() noexcept = default;

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem& operator=(const MeshSystem&) = delete;
		MeshSystem(MeshSystem&&) = delete;
		MeshSystem& operator=(MeshSystem&&) = delete;

		void ProcessComponents(const Entity* const _entity);
		void UpdateSubMeshCache();
		const std::vector<MeshData>& GetAllSubMeshes() const noexcept { return m_CachedSubMeshes; }
		const std::vector<MeshData>& GetSubMeshes(const uint32 _bufferId) const;
		VulkanVertexBuffer* GetVertexBuffer(const uint32 _bufferId);

	private:
		void AddMeshes(std::vector<MeshData>& _meshes);
		void AddMesh(MeshData& _mesh);

	private:
		VulkanVertexBufferManager& m_VertexBufferManager;
		std::vector<MeshData> m_CachedSubMeshes;
		std::unordered_map<uint32, std::vector<MeshData>> m_VertexBufferIdToSubMeshes;
		uint32 m_TotalMeshCount;
	};
} // End of namespace