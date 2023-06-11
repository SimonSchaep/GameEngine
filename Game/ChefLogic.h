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

class ChefLogic final : public engine::BaseComponent, public engine::CollisionEventReceiver, public engine::Observer<EventType>
{
public:
	ChefLogic(engine::GameObject* pGameObject);
	virtual ~ChefLogic() = default;

	virtual void Initialize() override;

	virtual void Notify(EventType) override;

	bool IsDead()const { return m_IsDead; }

	engine::Event<EventType, ChefLogic*>* GetOnDeath()const {return m_OnDeath.get(); }

private:
	void Respawn();

	virtual void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider) override;

	bool m_IsDead{};

	int m_DeathSound{};

	glm::vec2 m_Startpos{};

	std::unique_ptr<engine::Event<EventType, ChefLogic*>> m_OnDeath{};
};