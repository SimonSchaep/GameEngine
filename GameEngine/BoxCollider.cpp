#include "BoxCollider.h"
#include <iostream>

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

	void engine::BoxCollider::CheckTrigger(Collider* pCollider)
	{
		//std::cout << "triggercheck\n";
		std::vector<Collider*>& currentTriggers = GetCurrentTriggers();
		if (std::find(currentTriggers.begin(), currentTriggers.end(), pCollider) != currentTriggers.end()) //if we triggered previous frame
		{
			if (pCollider->IsRectInCollider(GetShapeInWorld())) //and we are still triggering
			{
				GetOnTriggerStayEvent()->NotifyObservers(this, pCollider); //on trigger stay
			}
			else
			{
				GetOnTriggerExitEvent()->NotifyObservers(this, pCollider); //on trigger exit
				RemoveCurrentTrigger(pCollider);
			}
		}
		else if (pCollider->IsRectInCollider(GetShapeInWorld())) //else, if we are triggering
		{
			//std::cout << "trigger\n";
			GetOnTriggerEnterEvent()->NotifyObservers(this, pCollider); //on trigger enter
			GetOnTriggerStayEvent()->NotifyObservers(this, pCollider); //on trigger stay
			AddCurrentTrigger(pCollider);
		}
	}

	structs::Rect engine::BoxCollider::GetShapeInWorld() const
	{
		structs::Rect worldShape{ m_Shape };
		worldShape.BottomLeft += glm::vec2(GetGameObject()->GetTransform()->GetWorldPosition());
		return worldShape;
	}

}

