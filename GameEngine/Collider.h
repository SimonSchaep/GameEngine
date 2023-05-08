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
		Event<Collider*, Collider*>* GetOnTriggerEnterEvent() { return m_OnTriggerEnter.get(); }
		Event<Collider*, Collider*>* GetOnTriggerStayEvent() { return m_OnTriggerStay.get(); }
		Event<Collider*, Collider*>* GetOnTriggerExitEvent() { return m_OnTriggerExit.get(); }

		virtual void Initialize()override {};
		virtual void Update()override {};

		virtual bool IsRectInCollider(const structs::Rect& rect) = 0;
		virtual bool IsPointInCollider(const glm::vec2& point) = 0;
		virtual bool IsCircleInCollider(const structs::Circle& circle) = 0;
		virtual void CheckTrigger(Collider* pCollider) = 0;

	protected:
		std::vector<Collider*>& GetCurrentTriggers() { return m_CurrentTriggers; }
		void AddCurrentTrigger(Collider* pCollider) { m_CurrentTriggers.push_back(pCollider); }
		void RemoveCurrentTrigger(Collider* pCollider) { m_CurrentTriggers.erase(std::remove(m_CurrentTriggers.begin(), m_CurrentTriggers.end(), pCollider)); }

	private:
		std::unique_ptr<Event<Collider*, Collider*>> m_OnTriggerEnter{};
		std::unique_ptr<Event<Collider*, Collider*>> m_OnTriggerStay{};
		std::unique_ptr<Event<Collider*, Collider*>> m_OnTriggerExit{};

		std::vector<Collider*> m_CurrentTriggers{};
	};
}

