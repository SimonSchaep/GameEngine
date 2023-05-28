#include "GameState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "TimeManager.h"
#include <memory>

using namespace engine;

//Start menu
StartMenuState::StartMenuState(GameManager* pGameManager, engine::GameObject* pMenuGameObject)
	:GameState(pGameManager)
	, m_pMenuGameObject{ pMenuGameObject }
{
	m_pMenuGameObject->SetIsActive(false);
}

GameState* StartMenuState::Update()
{
	if (m_StartGame)
	{
		return GetGameManager()->GetGamePlayingState();
	}
	return nullptr;
}

void StartMenuState::OnEnter()
{
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_SPACE, InputManager::KeyState::up, std::make_unique<StartGameCommand>(this)));
	m_StartGame = false;
	m_pMenuGameObject->SetIsActive(true);
	TimeManager::GetInstance().SetTimePaused(true);
}

void StartMenuState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();
	m_pMenuGameObject->SetIsActive(false);
}


//Game playing
GamePlayingState::GamePlayingState(GameManager* pGameManager)
	:GameState(pGameManager)
{
	
}

GameState* GamePlayingState::Update()
{
	if (m_PauseGame)
	{
		return GetGameManager()->GetGamePausedState();
	}
	if (m_EndGame)
	{
		return GetGameManager()->GetLeaderboardState();
	}
	return nullptr;
}

void GamePlayingState::OnEnter()
{
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_ESCAPE, InputManager::KeyState::up, std::make_unique<PauseGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_P, InputManager::KeyState::up, std::make_unique<PauseGameCommand>(this)));

	//todo: make this in game event instead of input event
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_G, InputManager::KeyState::up, std::make_unique<EndGameCommand>(this)));

	m_PauseGame = false;
	m_EndGame = false;
	TimeManager::GetInstance().SetTimePaused(false);
}

void GamePlayingState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();
}


//Game paused
GamePausedState::GamePausedState(GameManager* pGameManager, engine::GameObject* pPauseMenuGameObject)
	:GameState(pGameManager)
	, m_pPauseMenuGameObject{pPauseMenuGameObject}
{
	m_pPauseMenuGameObject->SetIsActive(false);
}

GameState* GamePausedState::Update()
{
	if (m_ResumeGame)
	{
		return GetGameManager()->GetGamePlayingState();
	}
	return nullptr;
}

void GamePausedState::OnEnter()
{
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_SPACE, InputManager::KeyState::up, std::make_unique<ResumeGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_P, InputManager::KeyState::up, std::make_unique<ResumeGameCommand>(this)));

	m_ResumeGame = false;
	TimeManager::GetInstance().SetTimePaused(true);

	m_pPauseMenuGameObject->SetIsActive(true);
}

void GamePausedState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();
	m_pPauseMenuGameObject->SetIsActive(false);
}


//Leaderboard
LeaderboardState::LeaderboardState(GameManager* pGameManager, engine::GameObject* pLeaderboardGameObject)
	:GameState(pGameManager)
	,m_pLeaderboardGameObject{pLeaderboardGameObject}
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
