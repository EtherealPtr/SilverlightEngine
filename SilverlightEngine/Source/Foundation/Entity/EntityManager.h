#pragma once

#include "Entity.h"

namespace Silverlight
{
	class EntityManager
	{
    public:
        SILVERLIGHT_ENGINE static const std::vector<Entity*>& GetAllEntities() noexcept { return s_Entities; }
        static uint32 RegisterEntity(Entity* _entity);
        static void UpdateEntities(const float _deltaTime);

    private:
        static uint32 s_TotalEntities;
        static std::vector<Entity*> s_Entities;
	};
} // End of namespace