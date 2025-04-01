#include "CustomMeshComponent.h"
#include "Foundation/Entity/Entity.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/Paths/PathManager.h"

namespace Silverlight
{
	CustomMeshComponent::CustomMeshComponent(std::string_view _modelPath) : 
		MeshComponent{},
		m_ModelName{ g_ResourceManager.GetAssetName(_modelPath) },
		m_Data{}
	{}

	void CustomMeshComponent::AddMeshData(MeshData& _data)
	{
		_data.SetVertexBufferId(GetVertexBufferId());
		_data.SetEntityId(GetOwner()->GetUniqueId());
		_data.SetShaderType(GetShaderType());

		if (m_TintColor.has_value())
		{
			_data.SetDiffuseColor(m_TintColor.value());
		}

		m_Data.emplace_back(_data);
	}

	const std::string CustomMeshComponent::GetModelPath() const
	{
		const std::string_view modelsFolder{ "Models/" };
		return PathManager::GetEngineResDirPath() + modelsFolder.data() + m_ModelName;
	}
} // End of namespace