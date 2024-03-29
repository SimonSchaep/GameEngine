#pragma once
#include "BaseComponent.h"
#include "Structs.h"
#include "Event.h"
#include "ObservingPointer.h"
#include "Observer.h"
#include "Event.h"
#include <vector>

//todo: add circle collider

namespace engine
{
	class Collider : public BaseComponent, public Observer<Collider*>
	{
	public:
		enum class TriggerType
		{
			enter,
			exit,
			stay,
		};

		Collider(GameObject* pGameObject);
		virtual ~Collider();
		Event<TriggerType, Collider*, Collider*>* GetOnTriggerEvent() { return m_OnTrigger.get(); }

		virtual void Initialize()override {};
		virtual void Update()override {};

		virtual void OnDisable()override;

		virtual bool IsRectInCollider(const structs::Rect& rect) = 0;
		virtual bool IsPointInCollider(const glm::vec2& point) = 0;
		virtual bool IsCircleInCollider(const structs::Circle& circle) = 0;
		virtual void CheckTrigger(Collider* pCollider) = 0;

		void AddCurrentTrigger(Collider* pCollider);
		void RemoveCurrentTrigger(Collider* pCollider);

		Event<Collider*>* GetOnDisableEvent()const { return m_OnDisable.get(); }
		virtual void Notify(Collider* pCollider)override;

	protected:
		std::vector<ObservingPointer<Collider>>& GetCurrentTriggers() { return m_CurrentTriggers; }

	private:
		std::unique_ptr<Event<TriggerType, Collider*, Collider*>> m_OnTrigger{}; //enter, stay and exit

		std::vector<ObservingPointer<Collider>> m_CurrentTriggers{};

		std::unique_ptr<Event<Collider*>> m_OnDisable{};
	};
}

