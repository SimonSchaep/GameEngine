#include "Collider.h"
#include "GameObject.h"

engine::Collider::Collider(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnTrigger = std::make_unique<Event<TriggerType, Collider*, Collider*>>();

	pGameObject->RegisterCollider(this);
}