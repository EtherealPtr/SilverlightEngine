#include "EntityManager.h"

namespace Silverlight
{
    uint32 EntityManager::s_TotalEntities{ 0 };
    std::vector<Entity*> EntityManager::s_Entities{};

    uint32 EntityManager::RegisterEntity(Entity* _entity)
    {
        s_Entities.emplace_back(_entity);
        return static_cast<uint32>(s_Entities.size());
    }

    void EntityManager::UpdateEntities(const float _deltaTime)
    {
        for (auto& entity : s_Entities)
        {
            entity->Update(_deltaTime);
        }
    }
} // End of namespace