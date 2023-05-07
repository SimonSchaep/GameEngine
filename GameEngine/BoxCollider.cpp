#include "BoxCollider.h"

engine::BoxCollider::BoxCollider(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnCollisionEnter = std::make_unique<Event<>>();
	m_OnCollisionExit = std::make_unique<Event<>>();
}

bool engine::BoxCollider::IsRectInCollider(const structs::Rect& /*rect*/)
{
	return false;
}

bool engine::BoxCollider::IsPointInCollider(const glm::vec2& /*point*/)
{
	return false;
}

bool engine::BoxCollider::IsCircleInCollider(const structs::Circle& /*circle*/)
{
	return false;
}
