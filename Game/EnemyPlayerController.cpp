#include "EnemyPlayerController.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "MovementComponent.h"
#include "EnemyLogic.h"
#include "Scene.h"
#include "GameManager.h"

using namespace engine;

EnemyPlayerController::EnemyPlayerController(engine::GameObject* pGameObject)
	:MyPlayerController(pGameObject)
{
}

void EnemyPlayerController::Move(const glm::vec2& direction)
{
	if (!m_pEnemyLogic->IsDead() && !m_pEnemyLogic->IsFalling() && !m_pEnemyLogic->IsStunned() && !IsPaused())
	{
		MyPlayerController::Move(direction);
	}
}

void EnemyPlayerController::SetControlledObject(engine::GameObject* pControlledObject)
{
	MyPlayerController::SetControlledObject(pControlledObject);

	m_pEnemyLogic = pControlledObject->GetComponent<EnemyLogic>();
}