#include "PlayerLives.h"
#include "DieCommand.h"
#include "Event.h"

PlayerLives::PlayerLives(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_DieEvent = std::make_unique<Event<>>();
	m_ResetLivesEvent = std::make_unique<Event<>>();
}

void PlayerLives::Initialize()
{
	ResetLives();
}

void PlayerLives::Die()
{
	m_Lives = 0;
	m_DieEvent->NotifyObservers();
}

void PlayerLives::ResetLives()
{
	m_Lives = m_MaxLives;
	m_ResetLivesEvent->NotifyObservers();
}
