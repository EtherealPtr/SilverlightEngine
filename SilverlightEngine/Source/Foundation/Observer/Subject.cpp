#include "Subject.h"
#include "Observer.h"

namespace Silverlight
{
	void Subject::AddObserver(const std::shared_ptr<Observer>& _observer)
	{
		m_Observer.push_back(_observer);
	}

	void Subject::RemoveObserver(const std::shared_ptr<Observer>& _observer)
	{
		auto it = std::find(m_Observer.begin(), m_Observer.end(), _observer);
		if (it != m_Observer.end())
		{
			m_Observer.erase(it);
		}
	}

	void Subject::RemoveAllObservers() noexcept
	{
		m_Observer.clear();
	}

	void Subject::Notify()
	{
		for (const auto& observer : m_Observer)
		{
			observer->OnNotify();
		}
	}
} // End of namespace