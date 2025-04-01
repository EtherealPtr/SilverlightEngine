#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Platform.h"
#include <vector>
#include <memory>
#include <concepts>

namespace Silverlight
{
	class Component;
	class Entity;

	template<typename T>
	concept IsComponent = std::is_base_of<Component, T>::value;

	template<typename T>
	concept HasRequiredComponentMethods = requires(T _component, const Entity* const _entity)
	{
		{ _component.SetOwner(_entity) } -> std::same_as<void>;
		{ _component.OnComponentInitialized() } -> std::same_as<void>;
	};

	class Entity
	{
	public:
		SILVERLIGHT_ENGINE explicit Entity();
		SILVERLIGHT_ENGINE ~Entity() noexcept = default;

		SILVERLIGHT_ENGINE const uint32 GetUniqueId() const noexcept { return m_Id; }

		template<typename T, typename... Args>
			requires IsComponent<T> && HasRequiredComponentMethods<T>
		std::shared_ptr<T> AddComponent(Args&&... _args);

		template<typename T>
			requires IsComponent<T>
		std::shared_ptr<T> GetComponent() const noexcept;

		virtual void Update(const float _deltaTime) {}

	protected:
		uint32 m_Id;
		std::vector<std::shared_ptr<Component>> m_Components;
	};

	template<typename T, typename... Args>
		requires IsComponent<T> && HasRequiredComponentMethods<T>
	std::shared_ptr<T> Entity::AddComponent(Args&&... _args)
	{
		auto component{ std::make_shared<T>(std::forward<Args>(_args)...) };
		component->SetOwner(this); 
		component->OnComponentInitialized();
		m_Components.emplace_back(component);
		return component;
	}

	template<typename T>
		requires IsComponent<T>
	std::shared_ptr<T> Entity::GetComponent() const noexcept
	{
		for (const auto& component : m_Components)
		{
			if (auto castedComponent = std::dynamic_pointer_cast<T>(component))
			{
				return castedComponent;
			}
		}

		return nullptr;
	}
} // End of namespace