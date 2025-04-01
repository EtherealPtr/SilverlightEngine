#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Components/Transform/TransformComponent.h" 
#include <unordered_map>
#include <memory>

namespace Silverlight
{
	class Entity;

	class TransformationSystem
	{
	public:
		TransformationSystem() = default;

		TransformationSystem(const TransformationSystem&) = delete;
		TransformationSystem& operator=(const TransformationSystem&) = delete;
		TransformationSystem(TransformationSystem&&) = delete;
		TransformationSystem& operator=(TransformationSystem&&) = delete;

		void ProcessComponents(const Entity* const _entity);
		std::shared_ptr<TransformComponent> GetTransformComponent(const uint32 _entityId) const;
		glm::mat4 GetEntityModelMatrix(const uint32 _entityId) const;

	private:
		std::unordered_map<uint32, std::shared_ptr<TransformComponent>> m_EntityTransformMap;
	};
} // End of namespace