#include "Collider.h"

engine::Collider::Collider(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnCollisionEnter = std::make_unique<Event<Collider*, Collider*>>();
	m_OnCollisionExit = std::make_unique<Event<Collider*, Collider*>>();
}