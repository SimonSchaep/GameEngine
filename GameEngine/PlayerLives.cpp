#include "PlayerLives.h"
#include "DieCommand.h"
#include "CustomEvents.h"

PlayerLives::PlayerLives(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_DieEvent = std::make_unique<DieEvent>();
	m_ResetLivesEvent = std::make_unique<ResetLivesEvent>();
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
