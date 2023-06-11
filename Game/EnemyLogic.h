#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "ObservingPointer.h"
#include "Collider.h"
#include "Event.h"
#include "EventTypes.h"
#include "CollisionEventReceiver.h"
#include <glm/glm.hpp>

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

class FoodParent;

class EnemyLogic final : public engine::BaseComponent
	, public engine::CollisionEventReceiver
	, public engine::Observer<EventType>
	, public engine::Observer<engine::GameObject*, EventType>
{
public:
	EnemyLogic(engine::GameObject* pGameObject);
	virtual ~EnemyLogic() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Notify(EventType) override;
	virtual void Notify(engine::GameObject* pObject, EventType type) override;

	bool IsDead()const { return m_IsDead; }
	bool IsStunned()const { return m_IsStunned; }
	bool IsFalling()const { return m_IsFalling; }

	engine::Event<EventType, EnemyLogic*>* GetOnDeath()const { return m_OnDeath.get(); }

private:
	void Respawn();

	virtual void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider)override;
	virtual void HandleTriggerExit(engine::Collider* pOriginCollider, engine::Collider* pHitCollider)override;

	bool m_IsDead{};
	bool m_IsStunned{};
	bool m_IsFalling{};

	float m_StunnedDuration{3.f};
	float m_StunnedDurationTimer{};

	float m_RespawnDelay{1.5f};
	float m_RespawnDelayTimer{};

	glm::vec2 m_Startpos{};

	std::unique_ptr<engine::Event<EventType, EnemyLogic*>> m_OnDeath{};

	engine::ObservingPointer<FoodParent> m_Food{};

	int m_FallSound{};
	int m_DeathSound{};
};

