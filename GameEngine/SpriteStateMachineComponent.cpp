#include "SpriteStateMachineComponent.h"
#include "GameObject.h"
#include "SpriteState.h"
#include "Sprite.h"
#include "SpriteRenderComponent.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "TimeManager.h"

engine::SpriteStateMachineComponent::SpriteStateMachineComponent(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
}

engine::SpriteState* engine::SpriteStateMachineComponent::CreateAndAddState(std::shared_ptr<Sprite> sprite)
{
	auto state = std::make_unique<SpriteState>(sprite);
	SpriteState* pReturnValue = state.get();

	m_SpriteStates.emplace_back(std::move(state));
	return pReturnValue;
}

void engine::SpriteStateMachineComponent::EvaluateStates()
{
	if (!m_pSpriteRenderComponent || !m_CurrentState || TimeManager::GetInstance().GetTimePaused())return;

	auto newState = m_CurrentState->EvaluateConnections();
	if (newState) //might be good to replace with a while loop to keep checking connections of newer states
	{
		m_CurrentState = newState;
		m_pSpriteRenderComponent->SetSprite(m_CurrentState->GetSprite());
	}
}

void engine::SpriteStateMachineComponent::Initialize()
{
	if (!m_pSpriteRenderComponent)
	{
		m_pSpriteRenderComponent = GetGameObject()->GetComponent<SpriteRenderComponent>();

		if (!m_pSpriteRenderComponent)
		{
			ServiceLocator::GetLogger().LogLine("No SpriteRenderer found for SpriteStateMachine\nAdd one to the GameObject", LogType::warning);
			return;
		}
	}

	if (m_SpriteStates.size() > 0 && !m_CurrentState)
	{
		m_CurrentState = m_SpriteStates[0].get();

		m_pSpriteRenderComponent->SetSprite(m_CurrentState->GetSprite());
	}
}

void engine::SpriteStateMachineComponent::Update()
{
	EvaluateStates();
}

void engine::SpriteStateMachineComponent::ForceStateEvaluation()
{
	EvaluateStates();
}
