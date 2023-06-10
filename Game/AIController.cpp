#include "AIController.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "Scene.h"
#include "GameManager.h"

AIController::AIController(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
}

void AIController::Initialize()
{
	auto pGameManager = GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>();
	pGameManager->GetOnChefWon()->AddObserver(this);
	pGameManager->GetOnRespawnCharacters()->AddObserver(this);
	pGameManager->GetOnChefDied()->AddObserver(this);
}

void AIController::Update()
{
	if (m_pControlledGameObject)
	{
		ProcessAIDecisions();
	}
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

void AIController::Notify(EventType type)
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

void AIController::Notify(EventType type, ChefLogic*)
{
	if (type == EventType::chefDied)
	{
		m_IsPaused = true;
	}
}

void AIController::ProcessAIDecisions()
{
}
