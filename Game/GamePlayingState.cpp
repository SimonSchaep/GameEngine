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
	if (m_CheckEndGame)
	{
		m_CheckEndGame = false;
		auto chefs = GetGameManager()->GetScene()->FindAllGameObjectsWithTag("Chef");
		//if no chefs left -> they are all dead
		if (chefs.size() == 0)
		{
			return GetGameManager()->GetLeaderboardState();
		}		
	}
	return nullptr;
}

void GamePlayingState::OnEnter()
{
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_ESCAPE, InputManager::KeyState::up, std::make_unique<PauseGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_P, InputManager::KeyState::up, std::make_unique<PauseGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F1, InputManager::KeyState::up, std::make_unique<NextLevelCommand>(GetGameManager())));

	m_SpawnSpecialPickupTimer = m_SpawnSpecialPickupInterval;

	m_PauseGame = false;
	m_CheckEndGame = false;
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

void GamePlayingState::Notify(EventType type)
{
	if (type == EventType::respawnCharacters)
	{
		m_CheckEndGame = true;
	}
}
