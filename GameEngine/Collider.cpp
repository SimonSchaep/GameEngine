#include "Collider.h"
#include "GameObject.h"

engine::Collider::Collider(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnTriggerEnter = std::make_unique<Event<Collider*, Collider*>>();
	m_OnTriggerStay = std::make_unique<Event<Collider*, Collider*>>();
	m_OnTriggerExit = std::make_unique<Event<Collider*, Collider*>>();

	pGameObject->RegisterCollider(this);
}