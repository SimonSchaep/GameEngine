#include "GamePlayingState.h"
#include "LeaderboardState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "TimeManager.h"
#include "Scene.h"
#include "ChefLogic.h"

using namespace engine;

GamePlayingState::GamePlayingState(GameManager* pGameManager)
	:GameState(pGameManager)
{
}

GameState* GamePlayingState::Update()
{
	if (m_SpawnSpecialPickupTimer > 0)
	{
		m_SpawnSpecialPickupTimer -= TimeManager::GetInstance().GetDeltaTime();

		if (m_SpawnSpecialPickupTimer <= 0)
		{
			m_SpawnSpecialPickupTimer = m_SpawnSpecialPickupInterval;
			GetGameManager()->SpawnSpecialPickup();
		}
	}

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
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_P, InputManager::KeyState::up, std::make_unique<PauseGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F1, InputManager::KeyState::up, std::make_unique<NextLevelCommand>(GetGameManager())));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_G, InputManager::KeyState::up, std::make_unique<EndGameCommand>(this)));

	m_SpawnSpecialPickupTimer = m_SpawnSpecialPickupInterval;

	m_PauseGame = false;
	m_EndGame = false;
	TimeManager::GetInstance().SetTimePaused(false);

	GetGameManager()->GetOnRespawnCharacters()->AddObserver(this);
}

void GamePlayingState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();

	GetGameManager()->GetOnRespawnCharacters()->RemoveObserver(this);
}

void GamePlayingState::Reset()
{
	m_LivesLeft = 3;
	m_PepperLeft = 5;
}

void GamePlayingState::Notify(EventType type)
{
	if (type == EventType::respawnCharacters)
	{
		if (m_LivesLeft > 0)
		{
			m_LivesLeft--;
		}
		else
		{
			m_EndGame = true;
		}
	}
}
