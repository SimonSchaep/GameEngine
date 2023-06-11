#include "StartMenuState.h"
#include "GameManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MenuCommands.h"
#include "TimeManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "TextRenderComponent.h"
#include "LayersEnum.h"

using namespace engine;

StartMenuState::StartMenuState(GameManager* pGameManager)
	:GameState(pGameManager)
{
	auto pScene = GetGameManager()->GetGameObject()->GetScene();
	auto windowSize = Renderer::GetInstance().GetWindowSize();

	m_pMenuGameObject = pScene->CreateAndAddGameObject("StartMenu", GetGameManager()->GetGameObject());

	//Background
	auto pChild = pScene->CreateAndAddGameObject("Background", m_pMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(0, 0);
	auto pTextureRenderer = pChild->CreateAndAddComponent<TextureRenderComponent>();
	pTextureRenderer->SetTexture("black.png");
	pTextureRenderer->SetSize(windowSize);
	pTextureRenderer->SetLayer(Layer::uiBackground);

	//Text
	pChild = pScene->CreateAndAddGameObject("Text", m_pMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2, windowSize.y * 0.8f);
	auto pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("PRESS 1/2/3 TO START");
	pTextRenderer->SetFont("super-burger-time.ttf", 30);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::center);

	//single player
	pChild = pScene->CreateAndAddGameObject("Text", m_pMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 300, windowSize.y * 0.6f);
	pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("1. SINGLE PLAYER");
	pTextRenderer->SetFont("super-burger-time.ttf", 20);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::left);

	//co-op
	pChild = pScene->CreateAndAddGameObject("Text", m_pMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 300, windowSize.y * 0.5f);
	pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("2. CO-OP");
	pTextRenderer->SetFont("super-burger-time.ttf", 20);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::left);

	//versus
	pChild = pScene->CreateAndAddGameObject("Text", m_pMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 300, windowSize.y * 0.4f);
	pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("3. VERSUS");
	pTextRenderer->SetFont("super-burger-time.ttf", 20);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::left);

	//instructions
	pChild = pScene->CreateAndAddGameObject("Text", m_pMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 220, windowSize.y * 0.25f);
	pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("MOVE - WASD/LEFT THUMB");
	pTextRenderer->SetFont("super-burger-time.ttf", 14);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::center);

	pChild = pScene->CreateAndAddGameObject("Text", m_pMenuGameObject);
	pChild->GetTransform()->SetLocalPosition(windowSize.x / 2 + 220, windowSize.y * 0.25f);
	pTextRenderer = pChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("PEPPER - SPACE/CONTROLLER A");
	pTextRenderer->SetFont("super-burger-time.ttf", 14);
	pTextRenderer->SetLayer(Layer::uiText);
	pTextRenderer->SetTextAlignment(TextRenderComponent::TextAlignment::center);

	m_pMenuGameObject->SetIsActive(false);
}

void StartMenuState::StartGame(GameMode gameMode)
{
	m_StartGame = true;
	m_GameMode = gameMode;
}

GameState* StartMenuState::Update()
{
	if (m_StartGame)
	{
		return GetGameManager()->GetGamePlayingState();
	}
	return nullptr;
}

void StartMenuState::OnEnter()
{
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_1, InputManager::KeyState::up, std::make_unique<StartGameCommand>(this, GameMode::singleplayer)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_2, InputManager::KeyState::up, std::make_unique<StartGameCommand>(this, GameMode::coop)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_3, InputManager::KeyState::up, std::make_unique<StartGameCommand>(this, GameMode::versus)));
	m_StartGame = false;
	m_pMenuGameObject->SetIsActive(true);
	TimeManager::GetInstance().SetTimePaused(true);

	GetGameManager()->GetGamePlayingState()->Reset();
}

void StartMenuState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();
	if (m_StartGame)
	{
		GetGameManager()->StartGame(m_GameMode);
	}
	m_pMenuGameObject->SetIsActive(false);	
}