#pragma once

namespace Silverlight
{
	class Entity;

	class Component
	{
	public:
		Component() noexcept :
			m_Owner{ nullptr }
		{}
		virtual ~Component() noexcept = default;

		virtual void OnComponentInitialized() noexcept {};
		void SetOwner(const Entity* const _owner) noexcept { m_Owner = _owner; }
		const Entity* const GetOwner() noexcept { return m_Owner; }

	protected:
		const Entity* m_Owner;
	};
} // End of namespace