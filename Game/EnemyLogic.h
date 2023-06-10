#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "Collider.h"
#include "Event.h"
#include "EventTypes.h"
#include <glm/glm.hpp>

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

class EnemyLogic final : public engine::BaseComponent
	, public engine::Observer<engine::Collider::TriggerType, engine::Collider*, engine::Collider*>
	, public engine::Observer<EventType>
	, public engine::Observer<engine::GameObject*, bool>
{
public:
	EnemyLogic(engine::GameObject* pGameObject);
	virtual ~EnemyLogic() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Notify(engine::Collider::TriggerType triggerType, engine::Collider* pOriginCollider, engine::Collider* pHitCollider) override;
	virtual void Notify(EventType) override;
	virtual void Notify(engine::GameObject* pObject, bool isFalling) override;

	bool IsDead()const { return m_IsDead; }
	bool IsStunned()const { return m_IsStunned; }
	bool IsFalling()const { return m_IsFalling; }

	engine::Event<EventType, EnemyLogic*>* GetOnDeath()const { return m_OnDeath.get(); }

private:
	void Respawn();

	void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerExit(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerStay(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);

	bool m_IsDead{};
	bool m_IsStunned{};
	bool m_IsFalling{};

	float m_StunnedDuration{3.f};
	float m_StunnedDurationTimer{};

	float m_RespawnDelay{2.f};
	float m_RespawnDelayTimer{};

	glm::vec2 m_Startpos{};

	std::unique_ptr<engine::Event<EventType, EnemyLogic*>> m_OnDeath{};
};

