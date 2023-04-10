#include "PlayerPoints.h"
#include "CustomEvents.h"

PlayerPoints::PlayerPoints(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_AddPointsEvent = std::make_unique<AddPointsEvent>();
}

void PlayerPoints::AddPoints(int amount)
{
	m_Points += amount;
	m_AddPointsEvent->NotifyObservers(GetGameObject());
}
