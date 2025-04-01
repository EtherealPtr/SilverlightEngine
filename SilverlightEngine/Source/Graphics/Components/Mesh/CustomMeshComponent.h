#pragma once

#include "MeshComponent.h"
#include "Graphics/MeshData.h"
#include <vector>
#include <string>

namespace Silverlight
{
	class CustomMeshComponent : public MeshComponent
	{
	public:
		SILVERLIGHT_ENGINE CustomMeshComponent(std::string_view _modelPath);

		void AddMeshData(MeshData& _data);
		std::string_view GetModelName() const noexcept { return m_ModelName; }
		const std::string GetModelPath() const;
		std::vector<MeshData>& GetMeshData() noexcept { return m_Data; }

	private:
		std::string m_ModelName;
		std::vector<MeshData> m_Data;
	};
} // End of namespace