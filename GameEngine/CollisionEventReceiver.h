#pragma once
#include "Observer.h"
#include "Collider.h"

namespace engine
{
	class GameObject;

	//Users still need to call addobserver on any collider that they want to receive trigger events for
	class CollisionEventReceiver : public Observer<Collider::TriggerType, Collider*, Collider*>
	{
	public:
		CollisionEventReceiver() = default;
		virtual ~CollisionEventReceiver() = default;

		virtual void Notify(engine::Collider::TriggerType triggerType, engine::Collider* pOriginCollider, engine::Collider* pHitCollider) override;

	protected:
		virtual void HandleTriggerEnter(engine::Collider*, engine::Collider*) {}
		virtual void HandleTriggerExit(engine::Collider*, engine::Collider*) {}
		virtual void HandleTriggerStay(engine::Collider*, engine::Collider*) {}
	};
}

