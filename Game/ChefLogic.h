#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "Collider.h"
#include "Event.h"
#include "EventTypes.h"

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

class ChefLogic final : public engine::BaseComponent, engine::Observer<engine::Collider::TriggerType, engine::Collider*, engine::Collider*>
{
public:
	ChefLogic(engine::GameObject* pGameObject);
	virtual ~ChefLogic() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Notify(engine::Collider::TriggerType triggerType, engine::Collider* pOriginCollider, engine::Collider* pHitCollider) override;

	bool IsDead()const { return m_IsDead; }

	engine::Event<EventType>* GetOnDeath()const {return m_OnDeath.get(); }

private:
	void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerExit(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerStay(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);

	bool m_IsDead{};

	std::unique_ptr<engine::Event<EventType>> m_OnDeath{};
};

