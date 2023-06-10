#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "Collider.h"
#include "Event.h"
#include "EventTypes.h"
#include "CollisionEventReceiver.h"

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

class SpecialPickupLogic final : public engine::BaseComponent, public engine::CollisionEventReceiver, public engine::Observer<EventType>
{
public:
	SpecialPickupLogic(engine::GameObject* pGameObject);
	virtual ~SpecialPickupLogic() = default;

	virtual void Initialize() override;

	virtual void Notify(EventType) override;

private:
	virtual void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider)override;
};