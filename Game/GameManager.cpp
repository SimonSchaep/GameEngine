#include "GameManager.h"
#include "GameState.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextRenderComponent.h"
#include "ResourceManager.h"
#include "Font.h"
#include "Renderer.h"

using namespace engine;

GameManager::GameManager(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
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

GameManager::~GameManager() = default;

void GameManager::Initialize()
{
	m_pActiveGameState = GetStartMenuState();
	m_pActiveGameState->OnEnter();
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
