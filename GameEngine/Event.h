#pragma once
#include <vector>
#include "Observer.h"
#include "GameObject.h"

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
		std::vector<Observer<Args...>*> m_Observers;
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
		for (auto pObserver : m_Observers)
		{
			pObserver->Notify(args...);
		}
	}

}
