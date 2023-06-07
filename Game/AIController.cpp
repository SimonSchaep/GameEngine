#include "AIController.h"
#include "GameObject.h"
#include "TimeManager.h"

AIController::AIController(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
}

void AIController::Initialize()
{
}

void AIController::Update()
{
	ProcessAIDecisions();
}

void AIController::Move(const glm::vec2& direction)
{
	if (m_pControlledMovementComponent)
	{
		m_pControlledMovementComponent->Move(direction);
	}
}

void AIController::SetControlledObject(engine::GameObject* pControlledObject)
{
	m_pControlledMovementComponent = pControlledObject->GetComponent<MovementComponent>();
	if (pControlledObject->HasTag("Enemy"))
	{
		m_pControlledMovementComponent->SetIsEnemy(true);
	}
}

void AIController::ProcessAIDecisions()
{
	if (m_pControlledMovementComponent && m_pControlledMovementComponent->CanMoveUp())
	{
		MoveUp();
	}
	else if (m_pControlledMovementComponent && m_pControlledMovementComponent->CanMoveRight())
	{
		MoveRight();
	}
	else if (m_pControlledMovementComponent && m_pControlledMovementComponent->CanMoveLeft())
	{
		MoveLeft();
	}
	else if (m_pControlledMovementComponent && m_pControlledMovementComponent->CanMoveDown())
	{
		MoveDown();
	}
}
