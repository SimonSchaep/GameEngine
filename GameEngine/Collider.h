#pragma once
#include "BaseComponent.h"
#include "Structs.h"
#include "Event.h"
#include <vector>

namespace engine
{
	class Collider : public BaseComponent
	{
	public:
		Collider(GameObject* pGameObject);
		Event<Collider*, Collider*>* GetOnCollisionEnterEvent() { return m_OnCollisionEnter.get(); }
		Event<Collider*, Collider*>* GetOnCollisionExitEvent() { return m_OnCollisionExit.get(); }

		virtual bool IsRectInCollider(const structs::Rect& rect) = 0;
		virtual bool IsPointInCollider(const glm::vec2& point) = 0;
		virtual bool IsCircleInCollider(const structs::Circle& circle) = 0;
		virtual void CheckCollision(Collider* pCollider) = 0;

	protected:
		std::vector<Collider*>& GetCurrentCollisions() { return m_CurrentCollisions; };

	private:
		std::unique_ptr<Event<Collider*, Collider*>> m_OnCollisionEnter{};
		std::unique_ptr<Event<Collider*, Collider*>> m_OnCollisionExit{};

		std::vector<Collider*> m_CurrentCollisions{};
	};
}

