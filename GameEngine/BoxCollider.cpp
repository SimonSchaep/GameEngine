#include "BoxCollider.h"
#include <iostream>

//todo: implement other collision functions

namespace engine
{
	bool engine::BoxCollider::IsRectInCollider(const structs::Rect& rect)
	{
		return (GetShapeInWorld().bottomLeft.x < rect.bottomLeft.x + rect.width && GetShapeInWorld().bottomLeft.x + GetShapeInWorld().width > rect.bottomLeft.x &&
			GetShapeInWorld().bottomLeft.y + GetShapeInWorld().height > rect.bottomLeft.y && GetShapeInWorld().bottomLeft.y < rect.bottomLeft.y + rect.height);
	}

	bool engine::BoxCollider::IsPointInCollider(const glm::vec2& /*point*/)
	{
		return false;
	}

	bool engine::BoxCollider::IsCircleInCollider(const structs::Circle& /*circle*/)
	{
		return false;
	}

	//todo: optimize this function
	void engine::BoxCollider::CheckTrigger(Collider* pOtherCollider)
	{
		auto& currentTriggers = GetCurrentTriggers();
		//this find is very inefficient
		if (std::find(currentTriggers.begin(), currentTriggers.end(), pOtherCollider) != currentTriggers.end()) //if we triggered previous frame
		{
			if (pOtherCollider->IsRectInCollider(GetShapeInWorld())) //and we are still triggering
			{
				GetOnTriggerEvent()->NotifyObservers(TriggerType::stay, this, pOtherCollider); //on trigger stay
				pOtherCollider->GetOnTriggerEvent()->NotifyObservers(TriggerType::stay, pOtherCollider, this); //on trigger stay
			}
			else
			{
				GetOnTriggerEvent()->NotifyObservers(TriggerType::exit, this, pOtherCollider); //on trigger exit
				pOtherCollider->GetOnTriggerEvent()->NotifyObservers(TriggerType::exit, pOtherCollider, this); //on trigger exit
				RemoveCurrentTrigger(pOtherCollider);
				pOtherCollider->RemoveCurrentTrigger(this);
			}
		}
		else if (pOtherCollider->IsRectInCollider(GetShapeInWorld())) //else, if we are triggering
		{
			GetOnTriggerEvent()->NotifyObservers(TriggerType::enter, this, pOtherCollider); //on trigger enter			
			GetOnTriggerEvent()->NotifyObservers(TriggerType::stay, this, pOtherCollider); //on trigger stay
			pOtherCollider->GetOnTriggerEvent()->NotifyObservers(TriggerType::enter, pOtherCollider, this); //on trigger enter
			pOtherCollider->GetOnTriggerEvent()->NotifyObservers(TriggerType::stay, pOtherCollider, this); //on trigger stay
			AddCurrentTrigger(pOtherCollider);
			pOtherCollider->AddCurrentTrigger(this);
		}
	}

	structs::Rect engine::BoxCollider::GetShapeInWorld() const
	{
		structs::Rect worldShape{ m_Shape };
		worldShape.bottomLeft += glm::vec2(GetGameObject()->GetTransform()->GetWorldPosition());
		return worldShape;
	}

}

