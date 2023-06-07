#include "MyPlayerController.h"
#include "MoveCommands.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "PlayerLives.h"
#include "PlayerPoints.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace engine;

MyPlayerController::MyPlayerController(engine::GameObject* pGameObject)
	:PlayerController(pGameObject)
{
}

void MyPlayerController::Initialize()
{
	//controller
	BindControllerButtonToCommand(InputController::ControllerButton::DPadLeft, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{-1,0}));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadRight, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadUp, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadDown, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));

	BindControllerAxisToCommand(InputController::ControllerAxis::ThumbLX, std::make_unique<MoveAxisCommand>(this, glm::vec2{ 1,0 }));
	BindControllerAxisToCommand(InputController::ControllerAxis::ThumbLY, std::make_unique<MoveAxisCommand>(this, glm::vec2{ 0,1 }));

	//keyboard
	BindKeyboardButtonToCommand(SDL_SCANCODE_A, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ -1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_LEFT, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ -1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_D, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_RIGHT, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_W, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_UP, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_S, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_DOWN, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));
}

void MyPlayerController::Move(const glm::vec2& direction)
{
	if (m_pControlledMovementComponent)
	{
		m_pControlledMovementComponent->Move(direction);
	}
}

void MyPlayerController::SetControlledObject(engine::GameObject* pControlledObject)
{
	m_pControlledMovementComponent = pControlledObject->GetComponent<MovementComponent>();
	if (pControlledObject->HasTag("Enemy"))
	{
		m_pControlledMovementComponent->SetIsEnemy(true);
	}
}
