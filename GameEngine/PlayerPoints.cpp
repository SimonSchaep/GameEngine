#include "PlayerPoints.h"
#include "CustomEvents.h"

PlayerPoints::PlayerPoints(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_KilledEnemyEvent = std::make_unique<KilledEnemyEvent>();
}

void PlayerPoints::KilledEnemy()
{
	m_KilledEnemyEvent->NotifyObservers(GetGameObject());
}
