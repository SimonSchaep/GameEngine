#include "LeaderboardState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "TimeManager.h"

using namespace engine;

LeaderboardState::LeaderboardState(GameManager* pGameManager, engine::GameObject* pLeaderboardGameObject)
	:GameState(pGameManager)
	, m_pLeaderboardGameObject{ pLeaderboardGameObject }
{
	m_pLeaderboardGameObject->SetIsActive(false);
}

GameState* LeaderboardState::Update()
{
	return nullptr;
}

void LeaderboardState::OnEnter()
{
	m_pLeaderboardGameObject->SetIsActive(true);
	TimeManager::GetInstance().SetTimePaused(true);
}

void LeaderboardState::OnExit()
{
	m_pLeaderboardGameObject->SetIsActive(false);
}