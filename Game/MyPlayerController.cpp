#include "MyPlayerController.h"
#include "MoveCommands.h"
#include "SwitchCommand.h"
#include "DieCommand.h"
#include "ResetLivesCommand.h"
#include "KilledEnemyCommand.h"
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
	auto pScene = GetGameObject()->GetScene();
	m_pChef = pScene->FindGameObjectByName("Chef");
	m_pBean = pScene->FindGameObjectByName("Bean");

	SetControlChef(m_ControlChef);


	//controller
	BindControllerButtonToCommand(InputController::ControllerButton::DPadLeft, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{-1,0}));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadRight, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadUp, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));
	BindControllerButtonToCommand(InputController::ControllerButton::DPadDown, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));

	BindControllerAxisToCommand(InputController::ControllerAxis::ThumbLX, std::make_unique<MoveAxisCommand>(this, glm::vec2{ 1,0 }));
	BindControllerAxisToCommand(InputController::ControllerAxis::ThumbLY, std::make_unique<MoveAxisCommand>(this, glm::vec2{ 0,-1 }));

	BindControllerButtonToCommand(InputController::ControllerButton::ButtonA, InputManager::KeyState::up, std::make_unique<SwitchCommand>(this));
	BindControllerButtonToCommand(InputController::ControllerButton::ButtonB, InputManager::KeyState::up, std::make_unique<DieCommand>(this));
	BindControllerButtonToCommand(InputController::ControllerButton::ButtonX, InputManager::KeyState::up, std::make_unique<ResetLivesCommand>(this));
	BindControllerButtonToCommand(InputController::ControllerButton::ButtonY, InputManager::KeyState::up, std::make_unique<KilledEnemyCommand>(this));

	//keyboard
	BindKeyboardButtonToCommand(SDL_SCANCODE_A, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ -1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_LEFT, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ -1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_D, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_RIGHT, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 1,0 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_W, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_UP, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_S, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));
	BindKeyboardButtonToCommand(SDL_SCANCODE_DOWN, InputManager::KeyState::pressed, std::make_unique<MoveCommand>(this, glm::vec2{ 0,-1 }));

	BindKeyboardButtonToCommand(SDL_SCANCODE_SPACE, InputManager::KeyState::up, std::make_unique<SwitchCommand>(this));
	BindKeyboardButtonToCommand(SDL_SCANCODE_1, InputManager::KeyState::up, std::make_unique<DieCommand>(this));
	BindKeyboardButtonToCommand(SDL_SCANCODE_2, InputManager::KeyState::up, std::make_unique<ResetLivesCommand>(this));
	BindKeyboardButtonToCommand(SDL_SCANCODE_3, InputManager::KeyState::up, std::make_unique<KilledEnemyCommand>(this));
}

void MyPlayerController::Move(const glm::vec2& direction)
{
	if (m_pControlledMovementComponent)
	{
		m_pControlledMovementComponent->Move(direction);
	}
}

void MyPlayerController::Die()
{
	if (m_pControlledPlayerLivesComponent)
	{
		m_pControlledPlayerLivesComponent->Die();
	}
}

void MyPlayerController::ResetLives()
{
	if (m_pControlledPlayerLivesComponent)
	{
		m_pControlledPlayerLivesComponent->ResetLives();
	}
}

void MyPlayerController::KilledEnemy()
{
	if (m_pControlledPlayerPointsComponent)
	{
		m_pControlledPlayerPointsComponent->KilledEnemy();
	}
}

void MyPlayerController::SetControlChef(bool controlChef)
{
	m_ControlChef = controlChef;

	if (m_pChef && m_ControlChef)
	{
		m_pControlledMovementComponent = m_pChef->GetComponent<MovementComponent>();
		m_pControlledPlayerLivesComponent = m_pChef->GetComponent<PlayerLives>();
		m_pControlledPlayerPointsComponent = m_pChef->GetComponent<PlayerPoints>();
	}
	else if (m_pBean && !m_ControlChef)
	{
		m_pControlledMovementComponent = m_pBean->GetComponent<MovementComponent>();
		m_pControlledPlayerLivesComponent = m_pBean->GetComponent<PlayerLives>();
		m_pControlledPlayerPointsComponent = m_pBean->GetComponent<PlayerPoints>();
	}
}

void MyPlayerController::SwitchControlledGameObjects()
{
	SetControlChef(!m_ControlChef);
}
