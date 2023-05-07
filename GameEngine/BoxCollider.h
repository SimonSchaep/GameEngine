#pragma once
#include "Collider.h"
#include "Structs.h"
#include "Event.h"

namespace engine
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(GameObject* pGameObject) :Collider(pGameObject) {}

		virtual bool IsRectInCollider(const structs::Rect& rect) override;
		virtual bool IsPointInCollider(const glm::vec2& point) override;
		virtual bool IsCircleInCollider(const structs::Circle& circle) override;
		virtual void CheckCollision(Collider* pCollider) override;

	private:
		structs::Rect m_Shape{};
	};
}


