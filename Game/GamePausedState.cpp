#include "GamePausedState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "TimeManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextRenderComponent.h"
#include "LayersEnum.h"

using namespace engine;

GamePausedState::GamePausedState(GameManager* pGameManager)
	:GameState(pGameManager)
{
	auto pScene = GetGameManager()->GetGameObject()->GetScene();
	auto windowSize = Renderer::GetInstance().GetWindowSize();

	m_pPauseMenuGameObject = pScene->CreateAndAddGameObject("PauseMenu", GetGameManager()->GetGameObject());

	//Background
	auto pChild = pScene->CreateAndAddGameObject("Background", m_pPauseMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(0, 0);
	auto pTextureRenderer = pChild->CreateAndAddComponent<TextureRenderComponent>();
	pTextureRenderer->SetTexture("blackHalfAlpha.png");
	pTextureRenderer->SetSize(windowSize);
	pTextureRenderer->SetLayer(Layer::uiBackground);

	//Text
	pChild = pScene->CreateAndAddGameObject("Text", m_pPauseMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2, windowSize.y / 2 + 100);
	auto pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("PAUSED");
	pTextRenderer->SetFont("super-burger-time.ttf", 30);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::center);

	//Instructions
	pChild = pScene->CreateAndAddGameObject("Text", m_pPauseMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2, windowSize.y / 2);
	pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("P - RESUME");
	pTextRenderer->SetFont("super-burger-time.ttf", 20);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::center);

	pChild = pScene->CreateAndAddGameObject("Text", m_pPauseMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2, windowSize.y / 2 - 50);
	pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("ESCAPE - BACK TO MENU");
	pTextRenderer->SetFont("super-burger-time.ttf", 20);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::center);

	m_pPauseMenuGameObject->SetIsActive(false);
}

GameState* GamePausedState::Update()
{
	if (m_BackToMenu)
	{
		return GetGameManager()->GetStartMenuState();
	}
	if (m_ResumeGame)
	{
		return GetGameManager()->GetGamePlayingState();
	}
	return nullptr;
}

void GamePausedState::OnEnter()
{
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_ESCAPE, InputManager::KeyState::up, std::make_unique<PauseToMenuCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_P, InputManager::KeyState::up, std::make_unique<ResumeGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F1, InputManager::KeyState::up, std::make_unique<NextLevelCommand>(GetGameManager())));

	m_ResumeGame = false;
	m_BackToMenu = false;
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