#include "LightSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Light/LightComponent.h"

namespace Silverlight
{
    void LightSystem::ProcessComponents(const Entity* const _entity)
    {
        if (const auto lightComponent = _entity->GetComponent<LightComponent>())
        {
            AddLightComponent(lightComponent);
        }
    }

    void LightSystem::AddLightComponent(const std::shared_ptr<LightComponent>& _lightComponent)
    {
        m_LightComponents.emplace_back(_lightComponent);

        if (_lightComponent->IsDirectionalLight() && !m_DirectionalLight.has_value())
        {
            m_DirectionalLight = std::static_pointer_cast<DirectionalLightComponent>(_lightComponent);
        }
    }

    std::array<LightData, LightSystem::MaxLights> LightSystem::UpdateLightData(uint8& _outLightCount) const noexcept
    {
        std::array<LightData, MaxLights> lights{};
        _outLightCount = 0;

        for (const auto& lightComponent : m_LightComponents)
        {
            if (_outLightCount >= MaxLights)
            {
                break;
            }

            lightComponent->UpdatePosition();
            lights[_outLightCount++] = lightComponent->GetLightData();
        }

        return lights;
    }
} // namespace Silverlight