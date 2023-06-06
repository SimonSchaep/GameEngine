#include "ChefSpriteController.h"
#include "GameObject.h"
#include "SpriteStateMachineComponent.h"
#include "ResourceManager.h"
#include "SpriteState.h"
#include "MovementComponent.h"

using namespace engine;

ChefSpriteController::ChefSpriteController(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{	
}

void ChefSpriteController::Initialize()
{
	//create state machine
	m_pSpriteStateMachine = GetGameObject()->CreateAndAddComponent<SpriteStateMachineComponent>();

	//add states
	auto pStateIdle = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("Chef.png", 1, 1, .1f, 0, 0, true, false)));
	auto pStateRunningLeft = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefRunning.png", 3, 1, .1f, 0, 2, true, false)));
	auto pStateRunningRight = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefRunning.png", 3, 1, .1f, 0, 2, true, false, true)));
	auto pStateClimbingUp = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefClimbingUp.png", 3, 1, .1f, 0, 2, true, false)));
	auto pStateClimbingDown = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("ChefClimbingDown.png", 3, 1, .1f, 0, 2, true, false)));
	
	auto pStateWinning = m_pSpriteStateMachine->CreateAndAddState(std::move(ResourceManager::GetInstance().LoadSprite("Chef.png", 3, 1, .1f, 0, 2, true, false))); //todo: change texture


	//add connections

	auto pMovementComponent = GetGameObject()->GetParent()->GetComponent<MovementComponent>();

	//idle to ...
	pStateIdle->AddConnection(SpriteConnection{ pStateRunningLeft, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().x < -0.01f;
		}
		});
	pStateIdle->AddConnection(SpriteConnection{ pStateRunningRight, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().x > 0.01f;
		}
		});
	pStateIdle->AddConnection(SpriteConnection{ pStateClimbingUp, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().y > 0.01f;
		}
		});
	pStateIdle->AddConnection(SpriteConnection{ pStateClimbingDown, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().y < -0.01f;
		}
		});

	//back to idle
	pStateRunningLeft->AddConnection(SpriteConnection{ pStateIdle, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().x > -0.01f;
		}
		});
	pStateRunningRight->AddConnection(SpriteConnection{ pStateIdle, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().x < 0.01f;
		}
		});
	pStateClimbingUp->AddConnection(SpriteConnection{ pStateIdle, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().y < 0.01f;
		}
		});
	pStateClimbingDown->AddConnection(SpriteConnection{ pStateIdle, [pMovementComponent]()
		{
			return pMovementComponent->GetCurrentMovementDirection().y > -0.01f;
		}
		});


	//to winning
	pStateIdle->AddConnection(SpriteConnection{ pStateWinning, [this]()
		{
			return m_HasPlayerWon;
		}
		});
	pStateRunningLeft->AddConnection(SpriteConnection{ pStateWinning, [this]()
		{
			return m_HasPlayerWon;
		}
		});
	pStateRunningRight->AddConnection(SpriteConnection{ pStateWinning, [this]()
		{
			return m_HasPlayerWon;
		}
		});
	pStateClimbingUp->AddConnection(SpriteConnection{ pStateWinning, [this]()
		{
			return m_HasPlayerWon;
		}
		});
	pStateClimbingDown->AddConnection(SpriteConnection{ pStateWinning, [this]()
		{
			return m_HasPlayerWon;
		}
		});
}

//todo: still need to bind this event
void ChefSpriteController::Notify()
{
	m_HasPlayerWon = true;
	m_pSpriteStateMachine->ForceStateEvaluation();
}
