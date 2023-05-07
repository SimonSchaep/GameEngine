#include "CollisionManager.h"
#include "Collider.h"

void engine::CollisionManager::CheckCollisions()
{
	for (auto pCollider : m_Colliders)
	{
		for (auto pOtherCollider : m_Colliders)
		{
			pCollider->CheckCollision(pOtherCollider);
		}
	}
}
