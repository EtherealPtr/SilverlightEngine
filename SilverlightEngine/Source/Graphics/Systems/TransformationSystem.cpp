#include "TransformationSystem.h"
#include "Foundation/Entity/Entity.h"

namespace Silverlight
{
	void TransformationSystem::ProcessComponents(const Entity* const _entity)
	{
		if (const auto& transformComponent{ _entity->GetComponent<TransformComponent>() })
		{
			m_EntityTransformMap[_entity->GetUniqueId()] = transformComponent;
		}
	}

	std::shared_ptr<TransformComponent> TransformationSystem::GetTransformComponent(const uint32 _entityId) const
	{
		auto it{ m_EntityTransformMap.find(_entityId) };
		if (it != m_EntityTransformMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	glm::mat4 TransformationSystem::GetEntityModelMatrix(const uint32 _entityId) const
	{
		auto transform{ GetTransformComponent(_entityId) };
		if (transform)
		{
			return transform->GetModel();
		}

		return glm::mat4(1.0f);
	}
} // End of namespace