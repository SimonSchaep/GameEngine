#include "GamePausedState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "TimeManager.h"

using namespace engine;

GamePausedState::GamePausedState(GameManager* pGameManager, engine::GameObject* pPauseMenuGameObject)
	:GameState(pGameManager)
	, m_pPauseMenuGameObject{ pPauseMenuGameObject }
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
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F1, InputManager::KeyState::up, std::make_unique<NextLevelCommand>(GetGameManager())));

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