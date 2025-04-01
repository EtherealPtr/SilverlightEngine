#pragma once

#include <memory>
#include <vector>

namespace Silverlight
{
	class Observer;

	class Subject
	{
	public:
		Subject() = default;
		~Subject() = default;

		void AddObserver(const std::shared_ptr<Observer>& _observer);
		void RemoveObserver(const std::shared_ptr<Observer>& _observer);
		void RemoveAllObservers() noexcept;
		void Notify();

		Subject(const Subject&) = delete;
		Subject& operator=(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator=(Subject&&) = delete;

	private:
		std::vector<std::shared_ptr<Observer>> m_Observer;
	};
} // End of namespace