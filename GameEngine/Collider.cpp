#include "Collider.h"
#include "GameObject.h"

engine::Collider::Collider(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnTrigger = std::make_unique<Event<TriggerType, Collider*, Collider*>>();

	pGameObject->RegisterCollider(this);
}

engine::Collider::~Collider()
{
	GetGameObject()->RemoveCollider(this);
}

void engine::Collider::OnDisable()
{
	m_CurrentTriggers.clear();
}
