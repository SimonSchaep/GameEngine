#include "ChefPlayerController.h"
#include "GameObject.h"
#include "ThrowPepperComponent.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "MovementComponent.h"
#include "ThrowPepperCommand.h"

using namespace engine;

ChefPlayerController::ChefPlayerController(engine::GameObject* pGameObject)
	:MyPlayerController(pGameObject)
{
}

void ChefPlayerController::Initialize()
{
	MyPlayerController::Initialize();

	BindKeyboardButtonToCommand(SDL_SCANCODE_SPACE, InputManager::KeyState::up, std::make_unique<ThrowPepperCommand>(this));
}

void ChefPlayerController::Update()
{
	MyPlayerController::Update();

	auto& direction = GetMovementComponent()->GetCurrentMovementDirection();
	if (direction != glm::vec2{})
	{
		m_LastMovementDirection = direction;
	}
}

void ChefPlayerController::SetControlledObject(engine::GameObject* pControlledObject)
{
	MyPlayerController::SetControlledObject(pControlledObject);

	m_pThrowPepperComponent = pControlledObject->GetComponent<ThrowPepperComponent>();
	if (!m_pThrowPepperComponent)
	{
		ServiceLocator::GetLogger().LogLine("no throw pepper component on chef", LogType::debug);
	}
}

void ChefPlayerController::ThrowPepper()
{
	if (m_pThrowPepperComponent)
	{
		m_pThrowPepperComponent->ThrowPepper(m_LastMovementDirection);
	}
}
