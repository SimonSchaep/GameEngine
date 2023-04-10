#pragma once
#include <vector>
class Observer;
class GameObject;

class Event
{
public:
	Event() = default;
	virtual ~Event() = default;

	void AddObserver(Observer* pObserver);
	void RemoveObserver(Observer* pObserver);

	void NotifyObservers(GameObject* pGameObject);

private:
	std::vector<Observer*> m_Observers;
};

