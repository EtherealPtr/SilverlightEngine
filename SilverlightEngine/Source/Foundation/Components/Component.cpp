#include "Component.h"
#include "Foundation/Observer/Observer.h"

namespace Silverlight
{
	void Component::SetDirty(const bool _dirty)
	{
		m_Dirty = _dirty;

		if (m_Dirty)
		{
			NotifyObservers();
			m_Dirty = false;
		}
	}

	void Component::RegisterObserver(const std::shared_ptr<Observer>& _observer) noexcept
	{
		if (!_observer.get())
		{
			return;
		}

		m_Observers.emplace_back(_observer);
	}

	void Component::NotifyObservers() const
	{
		for (const auto& observer : m_Observers)
		{
			if (observer)
			{
				observer->OnNotify();
			}
		}
	}
} // End of namespace