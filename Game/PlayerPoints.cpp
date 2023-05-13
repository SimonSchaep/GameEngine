#include "PlayerPoints.h"
#include "Event.h"

using namespace engine;

PlayerPoints::PlayerPoints(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_KilledEnemyEvent = std::make_unique<Event<>>();
}

void PlayerPoints::KilledEnemy()
{
	m_KilledEnemyEvent->NotifyObservers();
}
