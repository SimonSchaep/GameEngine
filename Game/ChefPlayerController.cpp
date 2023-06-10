#include "ChefPlayerController.h"
#include "GameObject.h"
#include "ThrowPepperComponent.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "MovementComponent.h"
#include "ThrowPepperCommand.h"
#include "ChefLogic.h"

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

void ChefPlayerController::Move(const glm::vec2& direction)
{
	if (!m_IsDead)
	{
		MyPlayerController::Move(direction);
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

	if (m_pChefLogic)
	{
		m_pChefLogic->GetOnDeath()->RemoveObserver(this);
	}
	m_pChefLogic = pControlledObject->GetComponent<ChefLogic>();
	if (!m_pThrowPepperComponent)
	{
		ServiceLocator::GetLogger().LogLine("no chef logic component on chef", LogType::debug);
	}
	else
	{
		m_pChefLogic->GetOnDeath()->AddObserver(this);
	}
}

void ChefPlayerController::ThrowPepper()
{
	if (m_pThrowPepperComponent && !m_IsDead)
	{
		m_pThrowPepperComponent->ThrowPepper(m_LastMovementDirection);
	}
}

void ChefPlayerController::Notify(EventType)
{
	m_IsDead = true;
}
