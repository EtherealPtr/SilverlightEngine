#pragma once

#include "Foundation/Entity/Entity.h"
#include "Components.h"

namespace ExampleApp
{
	class ExampleCustomModel : public Silverlight::Entity
	{
	public:
		explicit ExampleCustomModel(const char* _modelPath, const glm::vec3& _pos = glm::vec3(0.0f))
		{
			m_MeshComp = AddComponent<Silverlight::CustomMeshComponent>(_modelPath);
			m_Transform = AddComponent<Silverlight::TransformComponent>();
			m_Transform->SetPosition(_pos);
		}

		void SetTintColor(const glm::vec4& _col)
		{
			m_MeshComp->SetTintColor(_col);
		}

	private:
		std::shared_ptr<Silverlight::TransformComponent> m_Transform;
		std::shared_ptr<Silverlight::CustomMeshComponent> m_MeshComp;
	};

	class ExampleCube : public Silverlight::Entity
	{
	public:
		ExampleCube(const glm::vec3& _pos = glm::vec3(0.0f), const glm::vec3& _tintColor = glm::vec3(1.0f), const char* _texturePath = nullptr)
		{
			auto meshComponent{ AddComponent<Silverlight::PrimitiveMeshComponent>(Silverlight::PrimitiveShapeEnum::CubeShape) };

			if (_texturePath)
			{
				meshComponent->SetTexture(_texturePath);
			}
			else
			{
				meshComponent->SetTintColor(glm::vec4(_tintColor.x, _tintColor.y, _tintColor.z, 1.0f));
			}

			m_Transform = AddComponent<Silverlight::TransformComponent>();
			m_Transform->SetPosition(_pos);
		}

		void SetPosition(const glm::vec3& _pos)
		{
			m_Transform->SetPosition(_pos);
		}

		void SetScale(const float _scale)
		{
			m_Transform->SetScale(glm::vec3(_scale));
		}

	private:
		std::shared_ptr<Silverlight::TransformComponent> m_Transform;
	};

	class ExampleQuad : public Silverlight::Entity
	{
	public:
		explicit ExampleQuad(const glm::vec3& _pos = glm::vec3(0.0f), const glm::vec3& _tintColor = glm::vec3(1.0f), const char* _texturePath = nullptr)
		{
			auto meshComponent{ AddComponent<Silverlight::PrimitiveMeshComponent>(Silverlight::PrimitiveShapeEnum::SquareShape) };

			if (_texturePath)
			{
				meshComponent->SetTexture(_texturePath);
			}
			else
			{
				meshComponent->SetTintColor(glm::vec4(_tintColor.x, _tintColor.y, _tintColor.z, 1.0f));
			}

			m_Transform = AddComponent<Silverlight::TransformComponent>();
			m_Transform->SetPosition(_pos);
			m_Transform->SetScale(glm::vec3(10.0f, 10.0f, 1.0f));
			m_Transform->SetRotation(glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)));
		}

	private:
		std::shared_ptr<Silverlight::TransformComponent> m_Transform;
	};
} // End of namespace