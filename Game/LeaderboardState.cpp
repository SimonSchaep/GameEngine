#include "LeaderboardState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ScoreManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "UITextRenderComponent.h"
#include "UITextureRenderComponent.h"
#include "NameInputBox.h"
#include "LayersEnum.h"

using namespace engine;

LeaderboardState::LeaderboardState(GameManager* pGameManager)
	:GameState(pGameManager)
{
	auto pScene = GetGameManager()->GetGameObject()->GetScene();
	auto windowSize = Renderer::GetInstance().GetWindowSize();

	m_pLeaderboardGameObject = pScene->CreateAndAddGameObject("LeaderboardMenu", GetGameManager()->GetGameObject());

	//Background
	auto pChild = pScene->CreateAndAddGameObject("Background", m_pLeaderboardGameObject);
	pChild->GetTransform()->SetLocalPosition(0,0);
	auto pTextureRenderer = pChild->CreateAndAddComponent<UITextureRenderComponent>();
	pTextureRenderer->SetTexture("black.png");
	pTextureRenderer->SetSize(windowSize);
	pTextureRenderer->SetLayer(Layer::uiBackground);

	//Text
	pChild = pScene->CreateAndAddGameObject("Text", m_pLeaderboardGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 200, windowSize.y * 0.8f);
	auto pTextRenderer = pChild->CreateAndAddComponent<UITextRenderComponent>();
	pTextRenderer->SetText("LEADERBOARD");
	pTextRenderer->SetFont("fonts/super-burger-time.ttf", 30);
	pTextRenderer->SetLayer(Layer::uiText);

	pChild = pScene->CreateAndAddGameObject("Text", m_pLeaderboardGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2, windowSize.y * 0.15f);
	pTextRenderer = pChild->CreateAndAddComponent<UITextRenderComponent>();
	pTextRenderer->SetText("ESCAPE - BACK TO MENU");
	pTextRenderer->SetFont("fonts/super-burger-time.ttf", 10);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(UITextRenderComponent::TextAlignment::center);

	//Name input
	pChild = pScene->CreateAndAddGameObject("Name Input", m_pLeaderboardGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 150, windowSize.y / 2 + 100);
	m_pNameInputBox = pChild->CreateAndAddComponent<NameInputBox>();

	//Highscores
	m_pHighScoresGameObject = pScene->CreateAndAddGameObject("Highscores", m_pLeaderboardGameObject);

	m_pLeaderboardGameObject->SetIsActive(false);
}

GameState* LeaderboardState::Update()
{
	if (m_BackToMenu)
	{
		return GetGameManager()->GetStartMenuState();
	}
	return nullptr;
}

void LeaderboardState::OnEnter()
{
	GetGameManager()->StopMusic();
	GetGameManager()->StopStartGameSound();
	GetGameManager()->PlayMusic();

	m_pLeaderboardGameObject->SetIsActive(true);
	m_pNameInputBox->SetIsActive(true);
	m_pHighScoresGameObject->SetIsActive(false);

	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_ESCAPE, InputManager::KeyState::up, std::make_unique<LeaderboardToMenuCommand>(this)));

	TimeManager::GetInstance().SetTimePaused(true);

	m_BackToMenu = false;

	m_pNameInputBox->GetOnNameEntered()->AddObserver(this);

	GetGameManager()->GetGamePlayingState()->HideHud();
}

void LeaderboardState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();

	m_pLeaderboardGameObject->SetIsActive(false);

	m_pNameInputBox->GetOnNameEntered()->RemoveObserver(this);
}

void LeaderboardState::Notify(EventType type, const std::string& name)
{
	if (type == EventType::nameEntered)
	{
		auto pScoreManager = GetGameManager()->GetGameObject()->GetComponent<ScoreManager>();

		pScoreManager->SaveHighScore(name);

		auto highScores = pScoreManager->GetHighscores();

		//Remove old scores
		for (auto pChild : m_pHighScoresGameObject->GetChildren())
		{
			pChild->MarkForDeletion(true);
		}

		//Add new scores
		auto pScene = GetGameManager()->GetGameObject()->GetScene();
		auto windowSize = Renderer::GetInstance().GetWindowSize();
		const int maxNameSize{ 15 };
		const int maxScoresDisplayed{ 10 };
		const int distanceBetweenScores{ 30 };
		for (int i{}; i < std::min(int(highScores.size()), maxScoresDisplayed);++i)
		{
			auto pChild = pScene->CreateAndAddGameObject("Text", m_pHighScoresGameObject);
			pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 250, windowSize.y * 0.7f - i * distanceBetweenScores);
			auto pTextRenderer = pChild->CreateAndAddComponent<UITextRenderComponent>();
			pTextRenderer->SetFont("fonts/super-burger-time.ttf", 14);
			pTextRenderer->SetLayer(Layer::uiText);
			//limit name size
			if (highScores[i].second.size() > maxNameSize)
			{
				highScores[i].second = highScores[i].second.substr(0, maxNameSize);
			}
			pTextRenderer->SetText(highScores[i].second);

			pChild = pScene->CreateAndAddGameObject("Text", m_pHighScoresGameObject);
			pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 + 250, windowSize.y * 0.7f - i * distanceBetweenScores);
			pTextRenderer = pChild->CreateAndAddComponent<UITextRenderComponent>();
			pTextRenderer->SetFont("fonts/super-burger-time.ttf", 14);
			pTextRenderer->SetLayer(Layer::uiText);
			pTextRenderer->SetTextAlignment(UITextRenderComponent::TextAlignment::right);
			pTextRenderer->SetText(std::to_string(highScores[i].first) + " PTS");
		}

		m_pNameInputBox->SetIsActive(false);
		m_pHighScoresGameObject->SetIsActive(true);
	}
}
