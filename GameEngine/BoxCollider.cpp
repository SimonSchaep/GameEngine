#include "BoxCollider.h"

//todo: implement other collision functions

namespace engine
{
	bool engine::BoxCollider::IsRectInCollider(const structs::Rect& rect)
	{
		return (GetShapeInWorld().BottomLeft.x < rect.BottomLeft.x + rect.Width && GetShapeInWorld().BottomLeft.x + GetShapeInWorld().Width > rect.BottomLeft.x &&
			GetShapeInWorld().BottomLeft.y + GetShapeInWorld().Height > rect.BottomLeft.y && GetShapeInWorld().BottomLeft.y < rect.BottomLeft.y + rect.Height);
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
			if (!pCollider->IsRectInCollider(GetShapeInWorld())) //and we are no longer colliding
			{
				GetOnCollisionExitEvent()->NotifyObservers(this, pCollider); //on collision exit
			}
		}
		else if (pCollider->IsRectInCollider(GetShapeInWorld())) //else, if we are colliding
		{
			GetOnCollisionEnterEvent()->NotifyObservers(this, pCollider); //on collision enter
		}
	}

	structs::Rect engine::BoxCollider::GetShapeInWorld() const
	{
		structs::Rect worldShape{ m_Shape };
		worldShape.BottomLeft += glm::vec2(GetGameObject()->GetTransform()->GetWorldPosition());
		return worldShape;
	}

}

