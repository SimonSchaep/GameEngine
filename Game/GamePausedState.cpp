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
	auto pTextChild = pScene->CreateAndAddGameObject("Text", m_pPauseMenuGameObject);
	pTextChild->GetTransform()->SetLocalPosition(windowSize.x / 2 - 50, windowSize.y / 2 - 10);
	auto pTextRenderer = pTextChild->CreateAndAddComponent<TextRenderComponent>();
	pTextRenderer->SetText("PAUSED");
	pTextRenderer->SetFont("super-burger-time.ttf", 40);
	pTextRenderer->SetLayer(Layer::uiText);

	m_pPauseMenuGameObject->SetIsActive(false);
}

GameState* GamePausedState::Update()
{
	if (m_ResumeGame)
	{
		return GetGameManager()->GetGamePlayingState();
	}
	return nullptr;
}

void GamePausedState::OnEnter()
{
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_SPACE, InputManager::KeyState::up, std::make_unique<ResumeGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_P, InputManager::KeyState::up, std::make_unique<ResumeGameCommand>(this)));
	m_Commands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F1, InputManager::KeyState::up, std::make_unique<NextLevelCommand>(GetGameManager())));

	m_ResumeGame = false;
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