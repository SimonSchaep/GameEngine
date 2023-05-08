#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "Collider.h"
#include "Event.h"
#include <vector>

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

using namespace engine;

class FoodParent : public BaseComponent, Observer<Collider::TriggerType, Collider*, Collider*>
{
public:
	FoodParent(GameObject* pGameObject);
	virtual ~FoodParent() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	void StartFall();

	virtual void Notify(Collider::TriggerType triggerType, Collider* pOriginCollider, Collider* pHitCollider) override;

	bool IsFalling() { return m_IsFalling; }
	bool ReachedPlate() { return m_ReachedPlate; }

	Event<GameObject*, bool>* GetFallEvent() { return m_FallEvent.get(); }

private:
	void HandleTriggerEnter(Collider* pOriginCollider, Collider* pHitCollider);
	void HandleTriggerExit(Collider* pOriginCollider, Collider* pHitCollider);
	void HandleTriggerStay(Collider* pOriginCollider, Collider* pHitCollider);

	void StopFall();

	//this would be done in a rigidbody component if we had one
	float m_FallVelocity{0};
	float m_FallAcceleration{-400};
	float m_BounceVelocity{ 50 };

	float m_YPosForFoodDown{-4};
	std::vector<GameObject*> m_FoodElements{};
	std::vector<bool> m_FoodElementStates{}; //true == fallen

	std::unique_ptr<Event<GameObject*, bool>> m_FallEvent{};

	bool m_IsFalling{};
	bool m_ReachedPlate{};
};

