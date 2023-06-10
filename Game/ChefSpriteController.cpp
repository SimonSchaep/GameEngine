#include "ChefSpriteController.h"
#include "GameObject.h"
#include "SpriteStateMachineComponent.h"
#include "SpriteRenderComponent.h"
#include "ResourceManager.h"
#include "SpriteState.h"
#include "MovementComponent.h"
#include "ThrowPepperComponent.h"
#include "Scene.h"
#include "GameManager.h"
#include "ChefLogic.h"

using namespace engine;

ChefSpriteController::ChefSpriteController(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{	
}

void ChefSpriteController::Initialize()
{
	auto pGameManager = GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>();
	pGameManager->GetOnChefWon()->AddObserver(this);

	GetGameObject()->GetParent()->GetComponent<ChefLogic>()->GetOnDeath()->AddObserver(this);

	m_pMovementComponent = GetGameObject()->GetParent()->GetComponent<MovementComponent>();
	m_pThrowPepperComponent = GetGameObject()->GetParent()->GetComponent<ThrowPepperComponent>();

	//create state machine
	m_pSpriteStateMachine = GetGameObject()->CreateAndAddComponent<SpriteStateMachineComponent>();

	//add states
	auto pIdleDown = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefIdleDown.png", 1, 1, .1f, 0, 0, true, false))); //start here

	auto pIdleLeft = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefIdleLeft.png", 1, 1, .1f, 0, 0, true, false)));
	auto pIdleRight = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefIdleLeft.png", 1, 1, .1f, 0, 0, true, false, true)));
	auto pIdleUp = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefIdleUp.png", 1, 1, .1f, 0, 0, true, false)));	
	
	auto pRunningLeft = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefRunning.png", 3, 1, .1f, 0, 2, true, false)));
	auto pRunningRight = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefRunning.png", 3, 1, .1f, 0, 2, true, false, true)));
	auto pClimbingUp = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefClimbingUp.png", 3, 1, .1f, 0, 2, true, false)));
	auto pClimbingDown = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefClimbingDown.png", 3, 1, .1f, 0, 2, true, false)));
	
	auto pThrowLeft = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefThrowLeft.png", 2, 1, .2f, 0, 1, false, false)));
	auto pThrowRight = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefThrowLeft.png", 2, 1, .2f, 0, 1, false, false, true)));
	auto pThrowUp = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefThrowUp.png", 2, 1, .2f, 0, 1, false, false)));
	auto pThrowDown = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefThrowDown.png", 2, 1, .2f, 0, 1, false, false)));

	auto pWinning = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefWinning.png", 2, 1, .4f, 0, 1, true, false)));
	auto pStartDeath = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefDeathStart.png", 4, 1, .2f, 0, 3, false, false)));
	auto pDeath = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefDeath.png", 2, 1, .2f, 0, 1, true, false)));


	//add connections

	//idle to ...
	AddIdleToLeft(pIdleLeft, pRunningLeft);
	AddIdleToRight(pIdleLeft, pRunningRight);
	AddIdleToUp(pIdleLeft, pClimbingUp);
	AddIdleToDown(pIdleLeft, pClimbingDown);

	AddIdleToLeft(pIdleRight, pRunningLeft);
	AddIdleToRight(pIdleRight, pRunningRight);
	AddIdleToUp(pIdleRight, pClimbingUp);
	AddIdleToDown(pIdleRight, pClimbingDown);

	AddIdleToLeft(pIdleUp, pRunningLeft);
	AddIdleToRight(pIdleUp, pRunningRight);
	AddIdleToUp(pIdleUp, pClimbingUp);
	AddIdleToDown(pIdleUp, pClimbingDown);

	AddIdleToLeft(pIdleDown, pRunningLeft);
	AddIdleToRight(pIdleDown, pRunningRight);
	AddIdleToUp(pIdleDown, pClimbingUp);
	AddIdleToDown(pIdleDown, pClimbingDown);

	//back to idle
	pRunningLeft->AddConnection(SpriteConnection{ pIdleLeft, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().x > -0.01f;
		}
		});
	pRunningRight->AddConnection(SpriteConnection{ pIdleRight, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().x < 0.01f;
		}
		});
	pClimbingUp->AddConnection(SpriteConnection{ pIdleUp, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().y < 0.01f;
		}
		});
	pClimbingDown->AddConnection(SpriteConnection{ pIdleDown, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().y > -0.01f;
		}
		});


	//throwing
	AddStateToThrow(pIdleLeft, pThrowLeft);
	AddStateToThrow(pIdleRight, pThrowRight);
	AddStateToThrow(pIdleUp, pThrowUp);
	AddStateToThrow(pIdleDown, pThrowDown);

	AddStateToThrow(pRunningLeft, pThrowLeft);
	AddStateToThrow(pRunningRight, pThrowRight);
	AddStateToThrow(pClimbingUp, pThrowUp);
	AddStateToThrow(pClimbingDown, pThrowDown);

	pThrowLeft->AddConnection(SpriteConnection{ pIdleLeft, [this]()
		{
			return !m_IsPlayerThrowing;
		}
		});
	pThrowRight->AddConnection(SpriteConnection{ pIdleRight, [this]()
		{
			return !m_IsPlayerThrowing;
		}
		});
	pThrowUp->AddConnection(SpriteConnection{ pIdleUp, [this]()
		{
			return !m_IsPlayerThrowing;
		}
		});
	pThrowDown->AddConnection(SpriteConnection{ pIdleDown, [this]()
		{
			return !m_IsPlayerThrowing;
		}
		});

	//to winning
	AddStateToWinning(pIdleLeft, pWinning);
	AddStateToWinning(pIdleRight, pWinning);
	AddStateToWinning(pIdleUp, pWinning);
	AddStateToWinning(pIdleDown, pWinning);
	AddStateToWinning(pRunningLeft, pWinning);
	AddStateToWinning(pRunningRight, pWinning);
	AddStateToWinning(pClimbingUp, pWinning);
	AddStateToWinning(pClimbingDown, pWinning);

	//to death
	AddStateToDeath(pIdleLeft, pStartDeath);
	AddStateToDeath(pIdleRight, pStartDeath);
	AddStateToDeath(pIdleUp, pStartDeath);
	AddStateToDeath(pIdleDown, pStartDeath);
	AddStateToDeath(pRunningLeft, pStartDeath);
	AddStateToDeath(pRunningRight, pStartDeath);
	AddStateToDeath(pClimbingUp, pStartDeath);
	AddStateToDeath(pClimbingDown, pStartDeath);

	auto pSpriteRenderer = GetGameObject()->GetComponent<SpriteRenderComponent>();
	pStartDeath->AddConnection(SpriteConnection{ pDeath, [pSpriteRenderer]()
		{
			return pSpriteRenderer->GetIsPaused();
		}
		});
}

void ChefSpriteController::Update()
{
	m_IsPlayerThrowing = m_pThrowPepperComponent->IsThrowing();
}

void ChefSpriteController::Notify(EventType eventType)
{
	if (eventType == EventType::chefWon)
	{
		m_HasPlayerWon = true;
		GetGameObject()->GetComponent<SpriteRenderComponent>()->SetUpdateWhenTimePaused(true);
	}
	else if (eventType == EventType::chefDied)
	{
		m_IsPlayerDead = true;
	}
	m_pSpriteStateMachine->ForceStateEvaluation();
}

void ChefSpriteController::AddIdleToLeft(engine::SpriteState* idleState, engine::SpriteState* runningState)
{
	idleState->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().x < -0.01f;
		}
		});
	
}

void ChefSpriteController::AddIdleToRight(engine::SpriteState* idleState, engine::SpriteState* runningState)
{
	idleState->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().x > 0.01f;
		}
		});
	
}

void ChefSpriteController::AddIdleToUp(engine::SpriteState* idleState, engine::SpriteState* runningState)
{
	idleState->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().y > 0.01f;
		}
		});
	
}

void ChefSpriteController::AddIdleToDown(engine::SpriteState* idleState, engine::SpriteState* runningState)
{
	idleState->AddConnection(SpriteConnection{ runningState, [this]()
		{
			return m_pMovementComponent->GetCurrentMovementDirection().y < -0.01f;
		}
		});
}

void ChefSpriteController::AddStateToThrow(engine::SpriteState* state, engine::SpriteState* throwState)
{
	state->AddConnection(SpriteConnection{ throwState, [this]()
		{
			return m_IsPlayerThrowing;
		}
		});
}

void ChefSpriteController::AddStateToWinning(engine::SpriteState* state, engine::SpriteState* winningState)
{
	state->AddConnection(SpriteConnection{ winningState, [this]()
		{
			return m_HasPlayerWon;
		}
		});
}

void ChefSpriteController::AddStateToDeath(engine::SpriteState* state, engine::SpriteState* deathState)
{
	state->AddConnection(SpriteConnection{ deathState, [this]()
		{
			return m_IsPlayerDead;
		}
		});
}
