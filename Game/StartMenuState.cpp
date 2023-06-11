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
	auto pTextChild = pScene->CreateAndAddGameObject("Text", m_pMenuGameObject);
	pTextChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 200, windowSize.y / 2 - 10);
	auto pTextRenderer = pTextChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("PRESS 1/2/3 TO START");
	pTextRenderer->SetFont("super-burger-time.ttf", 40);
	pTextRenderer->SetLayer(Layer::uiText);

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
}

void StartMenuState::OnExit()
{
	for (auto& command : m_Commands)
	{
		InputManager::GetInstance().RemoveCommand(command);
	}
	m_Commands.clear();
	m_pMenuGameObject->SetIsActive(false);
	if (m_StartGame)
	{
		GetGameManager()->StartGame(m_GameMode);
	}
}