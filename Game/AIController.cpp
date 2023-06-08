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
	m_pControlledGameObject = pControlledObject;
	if (m_pControlledGameObject)
	{
		m_pControlledMovementComponent = m_pControlledGameObject->GetComponent<MovementComponent>();
		if (m_pControlledMovementComponent && m_pControlledGameObject->HasTag("Enemy"))
		{
			m_pControlledMovementComponent->SetIsEnemy(true);
		}
	}	
}

void AIController::ProcessAIDecisions()
{
}
