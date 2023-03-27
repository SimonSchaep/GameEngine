#include "PlayerMovementController.h"
#include "MoveCommands.h"
#include "SwitchCommand.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

void PlayerMovementController::Initialize()
{
	auto pScene = SceneManager::GetInstance().GetActiveScene();
	m_pChef = pScene->FindGameObjectByName("Chef");
	m_pBean = pScene->FindGameObjectByName("Bean");

	if (m_pChef && m_ControlChef)
	{
		m_pControlledMovementComponent = m_pChef->GetComponent<MovementComponent>();
	}
	else if (m_pBean && !m_ControlChef)
	{
		m_pControlledMovementComponent = m_pBean->GetComponent<MovementComponent>();
	}


	//controller
	BindControllerButtonToCommand(InputController::ControllerButton::DPadLeft, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{-1,0}));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadRight, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadUp, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadDown, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));

	BindControllerAxisToCommand(InputController::ControllerAxis::ThumbLX, std::make_unique<MoveAxisCommand>(this, glm::vec2{ 1,0 }));
	BindControllerAxisToCommand(InputController::ControllerAxis::ThumbLY, std::make_unique<MoveAxisCommand>(this, glm::vec2{ 0,-1 }));

	BindControllerButtonToCommand(InputController::ControllerButton::ButtonA, InputManager::KeyState::up, std::make_unique<SwitchCommand>(this));

	//keyboard
	BindKeyboardButtonToCommand(SDL_SCANCODE_A, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ -1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_LEFT, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ -1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_D, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_RIGHT, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_W, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_UP, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_S, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_DOWN, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));

	BindKeyboardButtonToCommand(SDL_SCANCODE_SPACE, InputManager::KeyState::up, std::make_unique<SwitchCommand>(this));
}

void PlayerMovementController::Move(const glm::vec2& direction)
{
	if (m_pControlledMovementComponent)
	{
		m_pControlledMovementComponent->Move(direction);
	}	
}

void PlayerMovementController::SetControlChef(bool controlChef)
{
	m_ControlChef = controlChef;

	if (m_pChef && m_ControlChef)
	{
		m_pControlledMovementComponent = m_pChef->GetComponent<MovementComponent>();
	}
	else if (m_pBean && !m_ControlChef)
	{
		m_pControlledMovementComponent = m_pBean->GetComponent<MovementComponent>();
	}
}

void PlayerMovementController::SwitchControlledGameObjects()
{
	SetControlChef(!m_ControlChef);
}
