#pragma once
#include <vector>
#include "Observer.h"
#include "GameObject.h"
#include "ObservingPointer.h"

namespace engine
{
	template<typename... Args>
	class Event final
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		void AddObserver(Observer<Args...>* pObserver);
		void RemoveObserver(Observer<Args...>* pObserver);

		void NotifyObservers(Args... args);

	private:
		std::vector<ObservingPointer<Observer<Args...>>> m_Observers;
	};

	template<typename ...Args>
	inline void Event<Args...>::AddObserver(Observer<Args...>* pObserver)
	{
		m_Observers.emplace_back(pObserver);
	}

	template<typename ...Args>
	inline void Event<Args...>::RemoveObserver(Observer<Args...>* pObserver)
	{
		m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), pObserver));
	}

	template<typename ...Args>
	inline void Event<Args...>::NotifyObservers(Args... args)
	{
		bool containsNull{};
		for (size_t i{}; i < m_Observers.size(); ++i)
		{
			if (m_Observers[i])
			{
				m_Observers[i]->Notify(args...);
			}
			else
			{
				containsNull = true;
			}
		}

		if (containsNull)
		{
			m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), nullptr));
		}
	}

}
