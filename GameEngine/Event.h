#pragma once
#include <vector>

namespace engine
{

	class Observer;
	class GameObject;

	class Event final
	{
	public:
		Event(int id) 
			:m_Id{id}
		{}
		virtual ~Event() = default;

		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);

		void NotifyObservers(GameObject* pGameObject);

	private:
		std::vector<Observer*> m_Observers;
		int m_Id;
	};

}