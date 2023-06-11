#include "GamePlayingState.h"
#include "LeaderboardState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "TimeManager.h"
#include "Scene.h"
#include "ChefLogic.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"
#include "UITextRenderComponent.h"
#include "UITextureRenderComponent.h"
#include "Renderer.h"
#include "ScoreManager.h"
#include "LayersEnum.h"

using namespace engine;

GamePlayingState::GamePlayingState(GameManager* pGameManager)
	:GameState(pGameManager)
{
	m_ScoreManager = GetGameManager()->GetGameObject()->GetComponent<ScoreManager>();

	auto pScene = GetGameManager()->GetGameObject()->GetScene();
	auto windowSize = Renderer::GetInstance().GetWindowSize();

	m_pHudGameObject = pScene->CreateAndAddGameObject("Hud", GetGameManager()->GetGameObject());

	//parent objects
	m_pLevelParentObject = pScene->CreateAndAddGameObject("LevelsHud", m_pHudGameObject);
	m_pLivesParentObject = pScene->CreateAndAddGameObject("LivesHud", m_pHudGameObject);
	m_pPepperParentObject = pScene->CreateAndAddGameObject("PepperHud", m_pHudGameObject);

	m_pLevelParentObject->GetTransform()->SetWorldPosition({ windowSize.x * 0.9f, windowSize.y * 0.05f });
	m_pLivesParentObject->GetTransform()->SetWorldPosition({ windowSize.x * 0.05f, windowSize.y * 0.05f });
	m_pPepperParentObject->GetTransform()->SetWorldPosition({ windowSize.x * 0.9f, windowSize.y * 0.95f });

	//score
	auto pChild = pScene->CreateAndAddGameObject("Text", m_pHudGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x * 0.05f, windowSize.y * 0.95f);
	auto pTextRenderer = pChild->CreateAndAddComponent<UITextRenderComponent>();
	pTextRenderer->SetText("SCORE");
	pTextRenderer->SetColor({255,0,0,255});
	pTextRenderer->SetFont("fonts/super-burger-time.ttf", 20);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(UITextRenderComponent::TextAlignment::left);

	pChild = pScene->CreateAndAddGameObject("Text", m_pHudGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x * 0.05f, windowSize.y * 0.9f);
	m_pScoreText = pChild->CreateAndAddComponent<UITextRenderComponent>();
	m_pScoreText->SetText("0");
	m_pScoreText->SetFont("fonts/super-burger-time.ttf", 20);
	m_pScoreText->SetLayer(Layer::uiText);
	m_pScoreText->SetTextAlignment(UITextRenderComponent::TextAlignment::left);

	//highscore
	pChild = pScene->CreateAndAddGameObject("Text", m_pHudGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x * 0.35f, windowSize.y * 0.95f);
	pTextRenderer = pChild->CreateAndAddComponent<UITextRenderComponent>();
	pTextRenderer->SetText("HIGHSCORE");
	pTextRenderer->SetColor({ 255,0,0,255 });
	pTextRenderer->SetFont("fonts/super-burger-time.ttf", 20);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(UITextRenderComponent::TextAlignment::left);

	pChild = pScene->CreateAndAddGameObject("Text", m_pHudGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x * 0.35f, windowSize.y * 0.9f);
	m_pHighScoreText = pChild->CreateAndAddComponent<UITextRenderComponent>();
	m_pHighScoreText->SetText("0");
	m_pHighScoreText->SetFont("fonts/super-burger-time.ttf", 20);
	m_pHighScoreText->SetLayer(Layer::uiText);
	m_pHighScoreText->SetTextAlignment(UITextRenderComponent::TextAlignment::left);

	m_pHudGameObject->SetIsActive(false);
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

	m_pScoreText->SetText(std::to_string(m_ScoreManager->GetScore()));

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
	m_pHudGameObject->SetIsActive(true);

	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_P, InputManager::KeyState::up, std::make_unique<PauseGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F1, InputManager::KeyState::up, std::make_unique<NextLevelCommand>(GetGameManager())));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F2, InputManager::KeyState::up, std::make_unique<EndGameCommand>(this)));

	m_SpawnSpecialPickupTimer = m_SpawnSpecialPickupInterval;

	m_PauseGame = false;
	m_EndGame = false;
	TimeManager::GetInstance().SetTimePaused(false);

	GetGameManager()->GetOnRespawnCharacters()->AddObserver(this);
	GetGameManager()->GetOnStartNextLevel()->AddObserver(this);
}

void GamePlayingState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();

	GetGameManager()->GetOnRespawnCharacters()->RemoveObserver(this);
	GetGameManager()->GetOnStartNextLevel()->RemoveObserver(this);
}

void GamePlayingState::AddPepper(int amount)
{
	m_PepperLeft += amount;
	UpdateDynamicHUD();
}

void GamePlayingState::Reset()
{
	m_LivesLeft = 3;
	m_PepperLeft = 5;
	m_Level = 0;

	auto scores = m_ScoreManager->GetHighscores();
	if (scores.size() == 0)
	{
		m_pHighScoreText->SetText("0");
	}
	else
	{
		m_pHighScoreText->SetText(std::to_string(scores[0].first));
	}
	UpdateDynamicHUD();
}

void GamePlayingState::HideHud()
{
	m_pHudGameObject->SetIsActive(false);
}

void GamePlayingState::Notify(EventType type)
{
	if (type == EventType::respawnCharacters)
	{
		if (m_LivesLeft > 0)
		{
			m_LivesLeft--;
			UpdateDynamicHUD();
		}
		else
		{
			m_EndGame = true;
		}
	}
	else if (type == EventType::startNextLevel)
	{
		m_Level++;
		UpdateDynamicHUD();
	}
}

void GamePlayingState::UpdateDynamicHUD()
{
	auto pScene = GetGameManager()->GetGameObject()->GetScene();

	//Remove old icons
	for (auto pChild : m_pLevelParentObject->GetChildren())
	{
		pChild->MarkForDeletion(true);
	}
	for (auto pChild : m_pLivesParentObject->GetChildren())
	{
		pChild->MarkForDeletion(true);
	}
	for (auto pChild : m_pPepperParentObject->GetChildren())
	{
		pChild->MarkForDeletion(true);
	}

	//level
	float distanceBetween{32};
	int levels{ m_Level };
	for (int i{}; i < levels; ++i)
	{
		
		auto pChild = pScene->CreateAndAddGameObject("LevelIcon", m_pLevelParentObject);
		pChild->GetTransform()->SetLocalPosition(0, i * distanceBetween);
		auto pTextureRenderer = pChild->CreateAndAddComponent<UITextureRenderComponent>();
		if (levels - i >= 10)
		{
			pTextureRenderer->SetTexture("lvlIcon10.png");
			levels -= 9;
		}
		else if (levels - i >= 5)
		{
			pTextureRenderer->SetTexture("lvlIcon5.png");
			levels -= 4;
		}
		else
		{
			pTextureRenderer->SetTexture("lvlIcon.png");
		}
		pTextureRenderer->SetSize({ 28,28 });
		pTextureRenderer->SetLayer(Layer::hud);
	}

	//lives
	distanceBetween = 32;
	for (int i{}; i < m_LivesLeft; ++i)
	{
		auto pChild = pScene->CreateAndAddGameObject("LifeIcon", m_pLivesParentObject);
		pChild->GetTransform()->SetLocalPosition(0, i * distanceBetween);
		auto pTextureRenderer = pChild->CreateAndAddComponent<UITextureRenderComponent>();
		pTextureRenderer->SetTexture("lifeIcon.png");
		pTextureRenderer->SetSize({ 28,28 });
		pTextureRenderer->SetLayer(Layer::hud);
	}


	//pepper
	distanceBetween = 32;
	for (int i{}; i < m_PepperLeft; ++i)
	{
		auto pChild = pScene->CreateAndAddGameObject("PepperIcon", m_pPepperParentObject);
		pChild->GetTransform()->SetLocalPosition(-i * distanceBetween,0);
		auto pTextureRenderer = pChild->CreateAndAddComponent<UITextureRenderComponent>();
		pTextureRenderer->SetTexture("pepperIcon.png");
		pTextureRenderer->SetSize({ 28,28 });
		pTextureRenderer->SetLayer(Layer::hud);
	}
}
