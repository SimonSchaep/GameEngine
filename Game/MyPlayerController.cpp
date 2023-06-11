#include "MyPlayerController.h"
#include "MoveCommands.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameManager.h"

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


	auto pGameManager = GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>();
	pGameManager->GetOnChefWon()->AddObserver(this);
	pGameManager->GetOnRespawnCharacters()->AddObserver(this);
	pGameManager->GetOnChefDied()->AddObserver(this);
}

void MyPlayerController::Move(const glm::vec2& direction)
{
	if (m_pControlledMovementComponent && !IsPaused())
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

void MyPlayerController::Notify(EventType type)
{
	if (type == EventType::chefWon)
	{
		m_IsPaused = true;
	}
	if (type == EventType::respawnCharacters)
	{
		m_IsPaused = false;
	}
}

void MyPlayerController::Notify(EventType type, ChefLogic*)
{
	if (type == EventType::chefDied)
	{
		m_IsPaused = true;
	}
}
