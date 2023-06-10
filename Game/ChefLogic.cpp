#include "ChefLogic.h"

using namespace engine;

ChefLogic::ChefLogic(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_DieEvent = std::make_unique<Event<GameObject*>>();
}

void ChefLogic::Initialize()
{
}

void ChefLogic::Update()
{
}

void ChefLogic::Notify(engine::Collider::TriggerType triggerType, engine::Collider* pOriginCollider, engine::Collider* pHitCollider)
{
	switch (triggerType)
	{
	case engine::Collider::TriggerType::enter:
		HandleTriggerEnter(pOriginCollider, pHitCollider);
		break;
	case engine::Collider::TriggerType::exit:
		HandleTriggerExit(pOriginCollider, pHitCollider);
		break;
	case engine::Collider::TriggerType::stay:
		HandleTriggerStay(pOriginCollider, pHitCollider);
		break;
	}
}

void ChefLogic::HandleTriggerEnter(engine::Collider* /*pOriginCollider*/, engine::Collider* pHitCollider)
{
	if (pHitCollider->HasTag("Enemy"))
	{
		m_IsDead = true;
	}
}

void ChefLogic::HandleTriggerExit(engine::Collider* /*pOriginCollider*/, engine::Collider* /*pHitCollider*/)
{
}

void ChefLogic::HandleTriggerStay(engine::Collider* /*pOriginCollider*/, engine::Collider* /*pHitCollider*/)
{
}
