#include "PlayerPoints.h"
#include "Event.h"
#include "EventId.h"

PlayerPoints::PlayerPoints(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_KilledEnemyEvent = std::make_unique<Event>(int(EventId::killedEnemy));
}

void PlayerPoints::KilledEnemy()
{
	m_KilledEnemyEvent->NotifyObservers(GetGameObject());
}
