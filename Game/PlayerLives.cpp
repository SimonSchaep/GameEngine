#include "PlayerLives.h"
#include "DieCommand.h"
#include "Event.h"
#include "EventId.h"

PlayerLives::PlayerLives(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_DieEvent = std::make_unique<Event>(int(EventId::playerDied));
	m_ResetLivesEvent = std::make_unique<Event>(int(EventId::playerResetLives));
}

void PlayerLives::Initialize()
{
	ResetLives();
}

void PlayerLives::Die()
{
	m_Lives = 0;
	m_DieEvent->NotifyObservers(GetGameObject());
}

void PlayerLives::ResetLives()
{
	m_Lives = m_MaxLives;
	m_ResetLivesEvent->NotifyObservers(GetGameObject());
}
