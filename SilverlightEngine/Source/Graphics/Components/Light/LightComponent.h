#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Components/Component.h"
#include "LightData.h"

namespace Silverlight
{
	class LightComponent : public Component
	{
	public:
		const LightData& GetLightData() const noexcept { return m_LightData; }
		virtual void UpdatePosition() noexcept {};
		virtual bool IsDirectionalLight() const noexcept { return false; }

	protected:
		LightData m_LightData;
	};
} // End of namespace