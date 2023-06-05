#pragma once
#include "BaseComponent.h"
#include "Structs.h"
#include "Event.h"
#include <vector>

//todo: add circle collider

namespace engine
{
	class Collider : public BaseComponent
	{
	public:
		enum class TriggerType
		{
			enter,
			exit,
			stay,
		};

		Collider(GameObject* pGameObject);
		Event<TriggerType, Collider*, Collider*>* GetOnTriggerEvent() { return m_OnTrigger.get(); }

		virtual void Initialize()override {};
		virtual void Update()override {};

		virtual bool IsRectInCollider(const structs::Rect& rect) = 0;
		virtual bool IsPointInCollider(const glm::vec2& point) = 0;
		virtual bool IsCircleInCollider(const structs::Circle& circle) = 0;
		virtual void CheckTrigger(Collider* pCollider) = 0;

		void AddCurrentTrigger(Collider* pCollider) { m_CurrentTriggers.emplace_back(pCollider); }
		void RemoveCurrentTrigger(Collider* pCollider) { m_CurrentTriggers.erase(std::remove(m_CurrentTriggers.begin(), m_CurrentTriggers.end(), pCollider)); }

	protected:
		std::vector<Collider*>& GetCurrentTriggers() { return m_CurrentTriggers; }

	private:
		std::unique_ptr<Event<TriggerType, Collider*, Collider*>> m_OnTrigger{}; //enter, stay and exit

		std::vector<Collider*> m_CurrentTriggers{};
	};
}

