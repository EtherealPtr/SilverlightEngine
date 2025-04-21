#pragma once

#include "Foundation/Platform.h"
#include "Graphics/PrimitiveShapeEnum.h"
#include <unordered_map>
#include <vector>

namespace Silverlight
{
	class VulkanVertexBufferManager;
	class VulkanVertexBuffer;
	class MeshData;
	class Entity;

	class MeshSystem
	{
	public:
		explicit MeshSystem(VulkanVertexBufferManager& _vertexBufferManager) noexcept;
		~MeshSystem() noexcept = default;

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem& operator=(const MeshSystem&) = delete;
		MeshSystem(MeshSystem&&) = delete;
		MeshSystem& operator=(MeshSystem&&) = delete;

		uint32 CreatePrimitiveMesh(const PrimitiveShapeEnum _shape);
		void ProcessComponents(const Entity* const _entity);
		void UpdateSubMeshCache();
		[[nodiscard]] const std::vector<MeshData>& GetAllMeshes() const noexcept { return m_CachedMeshes; }
		[[nodiscard]] const std::vector<MeshData>& GetSubMeshes(const uint32 _bufferId) const noexcept;
		[[nodiscard]] VulkanVertexBuffer* GetVertexBuffer(const uint32 _bufferId) noexcept;
		[[nodiscard]] const MeshData& GetSpecialMesh(const uint32 _meshId) const;

	private:
		void AddMeshes(std::vector<MeshData>& _meshes);
		void AddMesh(MeshData& _mesh);

	private:
		VulkanVertexBufferManager& m_VertexBufferManager;
		std::vector<MeshData> m_CachedMeshes;
		std::unordered_map<uint32, std::vector<MeshData>> m_VertexBufferIdToSubMeshes;
		std::unordered_map<uint32, MeshData> m_SpecialMeshes;
		uint32 m_TotalMeshCount;
	};
} // End of namespace