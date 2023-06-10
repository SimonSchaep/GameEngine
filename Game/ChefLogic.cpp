#include "ChefLogic.h"
#include "GameObject.h"
#include "BoxCollider.h"

using namespace engine;

ChefLogic::ChefLogic(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnDeath = std::make_unique<Event<EventType>>();
}

void ChefLogic::Initialize()
{
	GetGameObject()->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);
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
	if (m_IsDead) return;

	if (pHitCollider->HasTag("Enemy"))
	{
		m_IsDead = true;
		m_OnDeath->NotifyObservers(EventType::chefDied);
	}
}

void ChefLogic::HandleTriggerExit(engine::Collider* /*pOriginCollider*/, engine::Collider* /*pHitCollider*/)
{
}

void ChefLogic::HandleTriggerStay(engine::Collider* /*pOriginCollider*/, engine::Collider* /*pHitCollider*/)
{
}
