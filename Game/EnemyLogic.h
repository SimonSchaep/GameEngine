#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "Collider.h"
#include "Event.h"

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

class EnemyLogic : public engine::BaseComponent, engine::Observer<engine::Collider::TriggerType, engine::Collider*, engine::Collider*>
{
public:
	EnemyLogic(engine::GameObject* pGameObject);
	virtual ~EnemyLogic() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Notify(engine::Collider::TriggerType triggerType, engine::Collider* pOriginCollider, engine::Collider* pHitCollider) override;

private:
	void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerExit(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerStay(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);

};

