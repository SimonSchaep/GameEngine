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
#include "ChefPlayerController.h"
#include "AIController.h"
#include "BeanAIController.h"
#include "Level.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "ThrowPepperComponent.h"

using namespace engine;

GameManager::GameManager(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{}

GameManager::~GameManager() = default;

void GameManager::Initialize()
{
	InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F1, InputManager::KeyState::up, std::move(std::make_unique<NextLevelCommand>(this)));

	InitializeUI();

	m_pActiveGameState = GetStartMenuState();
	m_pActiveGameState->OnEnter();

	StartNextLevel();
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

void GameManager::StartNextLevel()
{
	auto pLevel = CreateLevel(m_NextLevelId);
	SceneManager::GetInstance().SetActiveScene(pLevel);
	m_NextLevelId = (m_NextLevelId + 1) % m_MaxLevelId;
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

Scene* GameManager::CreateLevel(int id)
{
	auto pScene = SceneManager::GetInstance().CreateScene("level" + std::to_string(id), true);

	//Load level
	auto pLevelObject = pScene->CreateAndAddGameObject("Level");
	pLevelObject->CreateAndAddComponent<Level>("Data/level" + std::to_string(id+1) + ".csv");

	auto pChef1 = CreateChef(pScene);
	auto pPlayerController = CreateChefPlayerController(pScene);

	pPlayerController->SetControlledObject(pChef1);
	pPlayerController->UseKeyboard(true);
	pPlayerController->UseController(1);

	auto pChef2 = CreateChef(pScene);
	pPlayerController = CreateChefPlayerController(pScene);

	pPlayerController->SetControlledObject(pChef2);
	pPlayerController->UseKeyboard(false);
	pPlayerController->UseController(0);


	auto pBean = CreateBean(pScene);
	auto pAIController = CreateBeanAIController(pScene);
	pAIController->SetControlledObject(pBean);
	pAIController->AddTarget(pChef1);
	pAIController->AddTarget(pChef2);

	return pScene;
}

engine::GameObject* GameManager::CreateChef(engine::Scene* pScene)
{
	auto pChef = pScene->CreateAndAddGameObject("Chef");
	pChef->AddTag("Chef");
	pChef->GetTransform()->SetLocalPosition({ 96,596 });
	pChef->CreateAndAddComponent<ThrowPepperComponent>(5);
	auto pMovementComponent = pChef->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(150);
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

	return pChef;
}

engine::GameObject* GameManager::CreateBean(engine::Scene* pScene)
{
	auto pBean = pScene->CreateAndAddGameObject("Bean");
	pBean->AddTag("Enemy");
	pBean->GetTransform()->SetLocalPosition({ 288,76 });
	auto pMovementComponent = pBean->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(100);

	//visuals
	auto pBeanVisuals = pScene->CreateAndAddGameObject("BeanVisuals", pBean);
	auto pRenderComponent = pBeanVisuals->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("bean.png");
	pRenderComponent->SetSize({ 28, 28 });
	pRenderComponent->SetLayer(1);
	auto width = float(pRenderComponent->GetSize().x);
	auto height = float(pRenderComponent->GetSize().y);
	pBeanVisuals->GetTransform()->SetLocalPosition({ -width / 2, -4 });

	//collider
	auto pBoxCollider = pBean->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ -width / 2, -8, width, height });

	return pBean;
}

ChefPlayerController* GameManager::CreateChefPlayerController(engine::Scene* pScene)
{
	auto pPlayerControllerObject = pScene->CreateAndAddGameObject();
	auto pPlayerController = pPlayerControllerObject->CreateAndAddComponent<ChefPlayerController>();
	return pPlayerController;
}

BeanAIController* GameManager::CreateBeanAIController(engine::Scene* pScene)
{
	auto pAIcontrollerObject = pScene->CreateAndAddGameObject();
	auto pAIController = pAIcontrollerObject->CreateAndAddComponent<BeanAIController>();
	return pAIController;
}
