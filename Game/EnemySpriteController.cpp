#include "EnemySpriteController.h"
#include "GameObject.h"
#include "SpriteStateMachineComponent.h"
#include "SpriteRenderComponent.h"
#include "ResourceManager.h"
#include "SpriteState.h"
#include "MovementComponent.h"
#include "ServiceLocator.h"
#include "Logger.h"

using namespace engine;

EnemySpriteController::EnemySpriteController(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
}

void EnemySpriteController::Initialize()
{
	m_pMovementComponent = GetGameObject()->GetParent()->GetComponent<MovementComponent>();

	//create state machine
	m_pSpriteStateMachine = GetGameObject()->CreateAndAddComponent<SpriteStateMachineComponent>();

	//states
	auto pRunningLeft = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite(m_RunningSpriteName, 2, 1, .15f, 0, 1, true, false)));
	auto pRunningRight = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite(m_RunningSpriteName, 2, 1, .15f, 0, 1, true, false, true)));
	auto pClimbingUp = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite(m_ClimbingUpSpriteName, 2, 1, .15f, 0, 1, true, false)));
	auto pClimbingDown = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite(m_ClimbingDownSpriteName, 2, 1, .15f, 0, 1, true, false)));

	auto pDeath = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite(m_DeathSpriteName, 4, 1, .1f, 0, 3, false, false)));
	auto pStunned = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite(m_StunnedSpriteName, 2, 1, .4f, 0, 1, true, false)));


	//connections
	//can basically go from any state to any state, so lots of connections here
	
	//to left
	AddStateToLeft(pRunningRight, pRunningLeft);
	AddStateToLeft(pClimbingUp, pRunningLeft);
	AddStateToLeft(pClimbingDown, pRunningLeft);
	AddStateToLeft(pDeath, pRunningLeft);
	AddStateToLeft(pStunned, pRunningLeft);

	//to right
	AddStateToRight(pRunningLeft, pRunningRight);
	AddStateToRight(pClimbingUp, pRunningRight);
	AddStateToRight(pClimbingDown, pRunningRight);
	AddStateToRight(pDeath, pRunningRight);
	AddStateToRight(pStunned, pRunningRight);

	//to up
	AddStateToUp(pRunningLeft, pClimbingUp);
	AddStateToUp(pRunningRight, pClimbingUp);
	AddStateToUp(pClimbingDown, pClimbingUp);
	AddStateToUp(pDeath, pClimbingUp);
	AddStateToUp(pStunned, pClimbingUp);

	//to down
	AddStateToDown(pRunningLeft, pClimbingDown);
	AddStateToDown(pRunningRight, pClimbingDown);
	AddStateToDown(pClimbingUp, pClimbingDown);
	AddStateToDown(pDeath, pClimbingDown);
	AddStateToDown(pStunned, pClimbingDown);

	//to death
	AddStateToDeath(pRunningLeft, pDeath);
	AddStateToDeath(pRunningRight, pDeath);
	AddStateToDeath(pClimbingUp, pDeath);
	AddStateToDeath(pClimbingDown, pDeath);
	AddStateToDeath(pStunned, pDeath);

	//to stunned
	AddStateToStunned(pRunningLeft, pStunned);
	AddStateToStunned(pRunningRight, pStunned);
	AddStateToStunned(pClimbingUp, pStunned);
	AddStateToStunned(pClimbingDown, pStunned);
}

void EnemySpriteController::Update()
{

}

void EnemySpriteController::Notify()
{
}

void EnemySpriteController::AddStateToLeft(engine::SpriteState* state, engine::SpriteState* runningState)
{
	state->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return !m_IsDead && !m_IsStunned && m_pMovementComponent->GetCurrentMovementDirection().x < -0.01f;
		}
		});
}

void EnemySpriteController::AddStateToRight(engine::SpriteState* state, engine::SpriteState* runningState)
{
	state->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return !m_IsDead && !m_IsStunned && m_pMovementComponent->GetCurrentMovementDirection().x > 0.01f;
		}
		});
}

void EnemySpriteController::AddStateToUp(engine::SpriteState* state, engine::SpriteState* runningState)
{
	state->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return !m_IsDead && !m_IsStunned && m_pMovementComponent->GetCurrentMovementDirection().y > 0.01f;
		}
		});
}

void EnemySpriteController::AddStateToDown(engine::SpriteState* state, engine::SpriteState* runningState)
{
	state->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return !m_IsDead && !m_IsStunned && m_pMovementComponent->GetCurrentMovementDirection().y < -0.01f;
		}
		});
}

void EnemySpriteController::AddStateToDeath(engine::SpriteState* state, engine::SpriteState* deathState)
{
	state->AddConnection(SpriteConnection{ deathState, [this]()
		{
			return m_IsDead;
		}
		});
}

void EnemySpriteController::AddStateToStunned(engine::SpriteState* state, engine::SpriteState* stunnedState)
{
	state->AddConnection(SpriteConnection{ stunnedState, [this]()
		{
			return m_IsStunned;
		}
		});
}
