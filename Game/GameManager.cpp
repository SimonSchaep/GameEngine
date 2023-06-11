#include "GameManager.h"
#include "GameState.h"
#include "GamePlayingState.h"
#include "GamePausedState.h"
#include "LeaderboardState.h"
#include "StartMenuState.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextRenderComponent.h"
#include "Renderer.h"
#include "MovementComponent.h"
#include "PlayerLives.h"
#include "PlayerPoints.h"
#include <SpriteRenderComponent.h>
#include "ChefSpriteController.h"
#include <BoxCollider.h>
#include "MyPlayerController.h"
#include "ChefPlayerController.h"
#include "AIController.h"
#include "EnemyAIController.h"
#include "Level.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "ThrowPepperComponent.h"
#include "HotdogSpriteController.h"
#include "EggSpriteController.h"
#include "PickleSpriteController.h"
#include "ChefLogic.h"
#include "EnemyLogic.h"
#include "FoodParent.h"
#include "TimeManager.h"
#include "EnemyPlayerController.h"
#include "SpecialPickupLogic.h"
#include "LifeTimer.h"
#include "LayersEnum.h"
#include "SoundSystem.h"

using namespace engine;

GameManager::GameManager(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnStartNextLevel = std::make_unique<Event<EventType>>();
	m_OnChefWon = std::make_unique<Event<EventType>>();
	m_OnChefDied = std::make_unique<Event<EventType, ChefLogic*>>();
	m_OnRespawnCharacters = std::make_unique<Event<EventType>>();
	m_OnSpawnPickup = std::make_unique<Event<EventType, SpecialPickupLogic*>>();
}

GameManager::~GameManager() = default;

void GameManager::Initialize()
{
	m_WinSound = ServiceLocator::GetSoundSystem().AddClip("data/sounds/win.wav");
	m_MusicSound = ServiceLocator::GetSoundSystem().AddClip("data/sounds/music.wav");
	m_StartGameSound = ServiceLocator::GetSoundSystem().AddClip("data/sounds/gamestart.wav");

	InitializeStates();

	m_pActiveGameState = GetStartMenuState();
	m_pActiveGameState->OnEnter();

	m_pNextLevel = CreateLevel(m_NextLevelId);
}

void GameManager::Update()
{
	auto pNewState = m_pActiveGameState->Update();
	if (pNewState)
	{
		m_pActiveGameState->OnExit();
		pNewState->OnEnter();
		m_pActiveGameState = pNewState;
	}

	if (m_FoodsNeedUpdate) //update here, cause scene doesn't switch immediately on setactive, and ontransfer will be in the middle of switching scenes
		//todo: maybe make ontransfer happen all at once, after switching scenes
	{
		m_FoodsNeedUpdate = false;
		m_FoodsLeftinLevel.clear();
		auto pScene = GetScene();
		pScene->GetName();
		auto foodObjects = GetScene()->FindAllGameObjectsWithTag("foodparent");
		for (auto pObject : foodObjects)
		{
			m_FoodsLeftinLevel.push_back(pObject->GetComponent<FoodParent>());
			assert(m_FoodsLeftinLevel.back() != nullptr); //if this fails, some object has tag "foodparent" but no foodparent component
			m_FoodsLeftinLevel.back()->GetReachedPlateEvent()->AddObserver(this);
		}
		m_OnStartNextLevel->NotifyObservers(EventType::startNextLevel);
	}

	if (m_StartGameSoundDurationTimer > 0)
	{
		m_StartGameSoundDurationTimer -= TimeManager::GetInstance().GetUnPausedDeltaTime();

		if (m_StartGameSoundDurationTimer <= 0)
		{
			PlayMusic();
		}
	}
	if (m_RespawnCharactersDelayTimer > 0)
	{
		m_RespawnCharactersDelayTimer -= TimeManager::GetInstance().GetUnPausedDeltaTime();

		if (m_RespawnCharactersDelayTimer <= 0)
		{
			m_OnRespawnCharacters->NotifyObservers(EventType::respawnCharacters);
			PlayStartGameSound();
		}
	}
	if (m_StartNextLevelDelayTimer > 0)
	{
		m_StartNextLevelDelayTimer -= TimeManager::GetInstance().GetUnPausedDeltaTime();

		if (m_StartNextLevelDelayTimer <= 0)
		{
			StartNextLevel();
		}
	}
}

void GameManager::OnSceneTransferred()
{
	m_FoodsNeedUpdate = true;
}

void GameManager::StartNextLevel()
{
	if (GetScene() == m_pNextLevel)
	{
		m_pNextLevel = CreateLevel(m_NextLevelId);
	}

	SceneManager::GetInstance().SetActiveScene(m_pNextLevel);
	AssignControllers();
	m_NextLevelId = (m_NextLevelId + 1) % m_MaxLevelId;

	m_StartNextLevelDelayTimer = 0;
	m_RespawnCharactersDelayTimer = 0;
}

void GameManager::StartGame(GameMode gameMode)
{
	m_GameMode = gameMode;
	m_NextLevelId = 0;
	StartNextLevel();
}

void GameManager::SpawnSpecialPickup()
{
	const float width{24};
	const float height{24};

	auto pGameObject = GetScene()->CreateAndAddGameObject("SpecialPickup");
	pGameObject->AddTag("SpecialPickup");

	pGameObject->CreateAndAddComponent<LifeTimer>(10.f);
	auto pCollider = pGameObject->CreateAndAddComponent<BoxCollider>();
	pCollider->SetShape({ 0,0,width,height });
	auto pRenderer = pGameObject->CreateAndAddComponent<TextureRenderComponent>();
	pRenderer->SetLayer(Layer::pickup);
	pRenderer->SetSize({width,height});

	EventType pickupType{};
	int randomValue = rand() % 3;
	switch (randomValue)
	{
	case 0:
		pRenderer->SetTexture("iceCream.png");
		pickupType = EventType::iceCreamPickedUp;
		break;
	case 1:
		pRenderer->SetTexture("fries.png");
		pickupType = EventType::friesPickedUp;
		break;
	case 2:
		pRenderer->SetTexture("coffee.png");
		pickupType = EventType::coffeePickedUp;
		break;
	}

	auto pPickupLogic = pGameObject->CreateAndAddComponent<SpecialPickupLogic>(pickupType);

	auto pLevel = GetScene()->FindGameObjectByName("Level")->GetComponent<Level>();

	int randTileIndex{};
	int row{}, col{};
	const int maxTries{ 100 }; //to avoid (litterally) random lagspikes

	for (int i{}; i < maxTries; ++i)
	{
		randTileIndex = rand() % (pLevel->GetLevelWidth() * pLevel->GetLevelHeight());
		pLevel->GetRowColOfIndex(randTileIndex, row, col);
		if (pLevel->IsNavigable(row, col, false))
		{
			break;
		}
	}

	auto pos = pLevel->GetCenterOfCell(randTileIndex);
	pos.x -= width / 2;
	pos.y -= height / 2;

	pGameObject->GetTransform()->SetWorldPosition(pos);


	m_OnSpawnPickup->NotifyObservers(EventType::spawnedPickup, pPickupLogic);
}

void GameManager::Notify(FoodParent* pFood)
{
	m_FoodsLeftinLevel.erase(std::remove(m_FoodsLeftinLevel.begin(), m_FoodsLeftinLevel.end(), pFood));

	CheckIfChefWon();
}

void GameManager::Notify(EventType eventType, ChefLogic* pChef)
{
	if (eventType == EventType::chefDied)
	{
		m_RespawnCharactersDelayTimer = m_RespawnCharactersDelay;
		m_OnChefDied->NotifyObservers(eventType, pChef);
	}
}

void GameManager::PlayMusic()
{
	ServiceLocator::GetSoundSystem().Play(m_MusicSound, -1);
}

void GameManager::StopMusic()
{
	ServiceLocator::GetSoundSystem().Stop(m_MusicSound);
}

void GameManager::PlayStartGameSound()
{
	ServiceLocator::GetSoundSystem().Play(m_StartGameSound);
	m_StartGameSoundDurationTimer = m_StartGameSoundDuration;
}

void GameManager::InitializeStates()
{
	//Startmenu state
	m_StartMenuState = std::make_unique<StartMenuState>(this);

	//Game playing state
	m_GamePlayingState = std::make_unique<GamePlayingState>(this);

	//Game paused state	
	m_GamePausedState = std::make_unique<GamePausedState>(this);

	//Leaderboard state	
	m_LeaderboardState = std::make_unique<LeaderboardState>(this);
}

Scene* GameManager::CreateLevel(int id)
{
	auto pScene = SceneManager::GetInstance().CreateScene("level" + std::to_string(id), true);

	//Load level
	auto pLevelObject = pScene->CreateAndAddGameObject("Level");
	auto pLevel = pLevelObject->CreateAndAddComponent<Level>("Data/levels/level" + std::to_string(id+1) + ".csv");

	pLevel->BuildLevel();

	//ServiceLocator::GetLogger().LogLine("creating level " + std::to_string(id));

	return pScene;
}

void GameManager::AssignControllers()
{
	auto pChef = m_pNextLevel->FindGameObjectByName("Chef");

	auto pPlayerControllerObject = m_pNextLevel->CreateAndAddGameObject();
	auto pPlayerController = pPlayerControllerObject->CreateAndAddComponent<ChefPlayerController>();

	pChef->GetComponent<ChefLogic>()->GetOnDeath()->AddObserver(this);

	pPlayerController->SetControlledObject(pChef);
	pPlayerController->UseKeyboard(true);
	pPlayerController->UseController(1);

	if (m_GameMode == GameMode::coop)
	{
		auto pLevel = m_pNextLevel->FindGameObjectByName("Level")->GetComponent<Level>();

		int index = pLevel->GetIndexOfPos(pChef->GetTransform()->GetWorldPosition());

		auto adjacentTiles = pLevel->GetAdjacentNavigableTiles(index, false);

		if (adjacentTiles.size() == 0)
		{
			ServiceLocator::GetLogger().LogLine("No adjacent tiles found next to chef", LogType::error);
			return;
		}
		
		auto pos = pLevel->GetCenterOfCell(adjacentTiles[0]);

		auto pChef2 = pLevel->SpawnChef(pos, true);

		pPlayerControllerObject = m_pNextLevel->CreateAndAddGameObject();
		pPlayerController = pPlayerControllerObject->CreateAndAddComponent<ChefPlayerController>();

		pChef2->GetComponent<ChefLogic>()->GetOnDeath()->AddObserver(this);

		pPlayerController->SetControlledObject(pChef2);
		pPlayerController->UseKeyboard(false);
		pPlayerController->UseController(0);
	}


	const float startDelay{1};
	const float delayPerEnemy{2};
	auto enemies = m_pNextLevel->FindAllGameObjectsWithTag("Enemy");
	for (size_t i{}; i < enemies.size(); ++i)
	{
		if (i == 0 && m_GameMode == GameMode::versus)
		{
			auto pEnemyPlayerControllerObject = m_pNextLevel->CreateAndAddGameObject();
			auto pEnemyPlayerController = pEnemyPlayerControllerObject->CreateAndAddComponent<EnemyPlayerController>();

			pEnemyPlayerController->SetControlledObject(enemies[i]);
			pEnemyPlayerController->UseKeyboard(false);
			pEnemyPlayerController->UseController(0);
		}
		else
		{
			auto pEnemyControllerObject = m_pNextLevel->CreateAndAddGameObject();
			auto pEnemyController = pEnemyControllerObject->CreateAndAddComponent<EnemyAIController>(startDelay + (i * delayPerEnemy));
			pEnemyController->SetControlledObject(enemies[i]);
		}
	}
}

void GameManager::CheckIfChefWon()
{
	if (m_FoodsLeftinLevel.size() == 0)
	{
		m_StartNextLevelDelayTimer = m_StartNextLevelDelay;
		ServiceLocator::GetSoundSystem().Play(m_WinSound);
		m_OnChefWon->NotifyObservers(EventType::chefWon);
		m_pNextLevel = CreateLevel(m_NextLevelId); //todo: this could be put in a separate thread to mask that the level is loading
	}
}