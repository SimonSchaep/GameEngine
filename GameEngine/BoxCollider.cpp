#include "BoxCollider.h"

//todo: implement other collision functions

bool engine::BoxCollider::IsRectInCollider(const structs::Rect& rect)
{
	return (m_Shape.BottomLeft.x < rect.BottomLeft.x + rect.Width && m_Shape.BottomLeft.x + m_Shape.Width > rect.BottomLeft.x &&
		m_Shape.BottomLeft.y + m_Shape.Height > rect.BottomLeft.y && m_Shape.BottomLeft.y < rect.BottomLeft.y + rect.Height);
}

bool engine::BoxCollider::IsPointInCollider(const glm::vec2& /*point*/)
{
	return false;
}

bool engine::BoxCollider::IsCircleInCollider(const structs::Circle& /*circle*/)
{
	return false;
}

void engine::BoxCollider::CheckCollision(Collider* pCollider)
{
	std::vector<Collider*>& currentCollisions = GetCurrentCollisions();
	if (std::find(currentCollisions.begin(), currentCollisions.end(), pCollider) != currentCollisions.end()) //if we collided previous frame
	{
		if (!pCollider->IsRectInCollider(m_Shape)) //and we are no longer colliding
		{
			GetOnCollisionExitEvent()->NotifyObservers(this, pCollider); //on collision exit
		}
	}
	else if (pCollider->IsRectInCollider(m_Shape)) //else, if we are colliding
	{
		GetOnCollisionEnterEvent()->NotifyObservers(this, pCollider); //on collision enter
	}
}
