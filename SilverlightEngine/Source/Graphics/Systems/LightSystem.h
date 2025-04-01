#pragma once

#include <array>
#include <vector>
#include <optional>
#include "Graphics/Components/Light/LightComponent.h"
#include "Graphics/Components/Light/DirectionalLightComponent.h"

namespace Silverlight
{
    class Entity;

    class LightSystem
    {
    public:
        static constexpr uint8 MaxLights{ 25 };

        LightSystem() noexcept = default;
        ~LightSystem() noexcept = default;

        LightSystem(const LightSystem&) = delete;
        LightSystem& operator=(const LightSystem&) = delete;
        LightSystem(LightSystem&&) = delete;
        LightSystem& operator=(LightSystem&&) = delete;

        void ProcessComponents(const Entity* const _entity);
        std::array<LightData, MaxLights> UpdateLightData(uint8& outLightCount) const noexcept;
        [[nodiscard]] const std::optional<std::shared_ptr<DirectionalLightComponent>>& GetDirectionalLight() const noexcept { return m_DirectionalLight; }

    private:
        void AddLightComponent(const std::shared_ptr<LightComponent>& lightComponent);

    private:
        std::vector<std::shared_ptr<LightComponent>> m_LightComponents;
        std::optional<std::shared_ptr<DirectionalLightComponent>> m_DirectionalLight;
    };
} // namespace Silverlight
