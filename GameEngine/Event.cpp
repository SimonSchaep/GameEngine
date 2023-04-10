#include "Event.h"
#include "Observer.h"
#include "GameObject.h"

void Event::AddObserver(Observer* pObserver)
{
	m_Observers.push_back(pObserver);
}

void Event::RemoveObserver(Observer* pObserver)
{
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), pObserver));
}

void Event::NotifyObservers(GameObject* pGameObject)
{
	for (auto pObserver : m_Observers)
	{
		pObserver->Notify(this, pGameObject);
	}
}
