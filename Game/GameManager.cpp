#include "GameManager.h"
#include "GameState.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextRenderComponent.h"
#include "ResourceManager.h"
#include "Font.h"
#include "Renderer.h"
#include "MovementComponent.h"
#include "PlayerLives.h"
#include "PlayerPoints.h"
#include <SpriteRenderComponent.h>
#include "ChefSpriteController.h"
#include <BoxCollider.h>
#include "MyPlayerController.h"
#include "Level.h"

using namespace engine;

GameManager::GameManager(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{}

GameManager::~GameManager() = default;

void GameManager::Initialize()
{
	InitializeUI();

	m_pActiveGameState = GetStartMenuState();
	m_pActiveGameState->OnEnter();

	StartLevel1();
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
}

void GameManager::StartLevel1()
{
	auto pScene = SceneManager::GetInstance().CreateScene("Level1");
	
	//Load level
	auto pLevelObject = pScene->CreateAndAddGameObject("Level");
	pLevelObject->CreateAndAddComponent<Level>("Data/level1.csv");

	//create chef
	auto pChef = pScene->CreateAndAddGameObject("Chef");
	pChef->AddTag("Chef");
	pChef->GetTransform()->SetLocalPosition({ 96,596 });
	auto pMovementComponent = pChef->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(100);
	auto pPlayerLives = pChef->CreateAndAddComponent<PlayerLives>();
	pPlayerLives->SetMaxLives(5);
	pChef->CreateAndAddComponent<PlayerPoints>();

	//visuals
	auto pChefVisuals = pScene->CreateAndAddGameObject("ChefVisuals", pChef);
	auto pSpriteRenderComponent = pChefVisuals->CreateAndAddComponent<SpriteRenderComponent>();
	pSpriteRenderComponent->SetSize({ 28, 28 });

	pChefVisuals->CreateAndAddComponent<ChefSpriteController>();

	float width = float(pSpriteRenderComponent->GetSize().x);
	float height = float(pSpriteRenderComponent->GetSize().y);
	pChefVisuals->GetTransform()->SetLocalPosition({ -width / 2, -8 });
	
	//collider
	auto pBoxCollider = pChef->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ -width / 2, -8, width, height });


	//player 1 controller gameobject
	auto pPlayer1ControllerObject = pScene->CreateAndAddGameObject();
	auto pPlayer1Controller = pPlayer1ControllerObject->CreateAndAddComponent<MyPlayerController>();
	pPlayer1Controller->UseKeyboard(true);
	pPlayer1Controller->UseController(1);
	//make controller posses chef
	pPlayer1Controller->SetControlChef(true);


	SceneManager::GetInstance().SetActiveScene(pScene);
}

void GameManager::InitializeUI()
{
	auto pScene = GetGameObject()->GetScene();
	auto windowSize = Renderer::GetInstance().GetWindowSize();

	//Startmenu state	
	auto pStartMenu = pScene->CreateAndAddGameObject("StartMenu", GetGameObject());
	auto pTextChild = pScene->CreateAndAddGameObject("Text", pStartMenu);
	pTextChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 200, windowSize.y / 2 - 10);
	auto pTextRenderer = pTextChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("PRESS SPACE TO START");
	pTextRenderer->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 40));
	m_StartMenuState = std::make_unique<StartMenuState>(this, pStartMenu);

	//Game playing state
	m_GamePlayingState = std::make_unique<GamePlayingState>(this);

	//Game paused state
	auto pPauseMenu = pScene->CreateAndAddGameObject("PauseMenu", GetGameObject());
	pTextChild = pScene->CreateAndAddGameObject("Text", pPauseMenu);
	pTextChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 50, windowSize.y / 2 - 10);
	pTextRenderer = pTextChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("PAUSED");
	pTextRenderer->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 40));
	m_GamePausedState = std::make_unique<GamePausedState>(this, pPauseMenu);

	//Leaderboard state
	auto pLeaderboardMenu = pScene->CreateAndAddGameObject("LeaderboardMenu", GetGameObject());
	pTextChild = pScene->CreateAndAddGameObject("Text", pLeaderboardMenu);
	pTextChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 50, windowSize.y / 2 - 10);
	pTextRenderer = pTextChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("LEADERBOARD");
	pTextRenderer->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 40));
	m_LeaderboardState = std::make_unique<LeaderboardState>(this, pLeaderboardMenu);
}
