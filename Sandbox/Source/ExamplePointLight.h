#pragma once

#include "Foundation/Entity/Entity.h"
#include "Graphics/PrimitiveShapeEnum.h"
#include "Components.h"

namespace ExampleApp
{
    class ExamplePointLight : public Silverlight::Entity
    {
    public:
        ExamplePointLight(const glm::vec3& _pos = glm::vec3(1.0f), const glm::vec4& _color = glm::vec4(1.0f), const float _radius = 15.0f, const glm::vec3& _orbitCenter = glm::vec3(0.0f, 1.0f, 0.0f)) :
            m_InitialPosition{ _pos },
            m_OrbitCenter{ _orbitCenter }
        {
            auto meshComp{ AddComponent<Silverlight::PrimitiveMeshComponent>(Silverlight::PrimitiveShapeEnum::CubeShape) };
            meshComp->SetTintColor(glm::vec4(_color.x, _color.y, _color.z, 1.0f));
            AddComponent<Silverlight::PointLightComponent>(_color, _radius);
            m_Transform = AddComponent<Silverlight::TransformComponent>();
            m_Transform->SetScale(glm::vec3(0.5f));
            m_Transform->SetPosition(_pos);
        }

        void Update(const float _deltaTime) override
        {
            static float timeElapsed{ 0.0f };
            timeElapsed += _deltaTime;

            const float speed{ 0.3f };
            float angle{ timeElapsed * speed };

            const glm::vec3 offset{ m_InitialPosition - m_OrbitCenter };
            const float orbitRadius{ glm::length(offset) };

            const glm::vec3 orbitDirection{ glm::normalize(offset) };

            const float x{ m_OrbitCenter.x + orbitRadius * cos(angle) * orbitDirection.x - orbitRadius * sin(angle) * orbitDirection.z };
            const float z{ m_OrbitCenter.z + orbitRadius * sin(angle) * orbitDirection.x + orbitRadius * cos(angle) * orbitDirection.z };

            m_Transform->SetPosition(glm::vec3(x, m_InitialPosition.y, z));
        }

    private:
        std::shared_ptr<Silverlight::TransformComponent> m_Transform;
        glm::vec3 m_InitialPosition;
        glm::vec3 m_OrbitCenter;
    };
} // End of namespace