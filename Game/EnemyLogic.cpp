#include "EnemyLogic.h"

using namespace engine;

EnemyLogic::EnemyLogic(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
}

void EnemyLogic::Initialize()
{
}

void EnemyLogic::Update()
{
}

void EnemyLogic::Notify(engine::Collider::TriggerType triggerType, engine::Collider* pOriginCollider, engine::Collider* pHitCollider)
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
	default:
		break;
	}
}

void EnemyLogic::HandleTriggerEnter(engine::Collider* /*pOriginCollider*/, engine::Collider* /*pHitCollider*/)
{
}

void EnemyLogic::HandleTriggerExit(engine::Collider* /*pOriginCollider*/, engine::Collider* /*pHitCollider*/)
{
}

void EnemyLogic::HandleTriggerStay(engine::Collider* /*pOriginCollider*/, engine::Collider* /*pHitCollider*/)
{
}
