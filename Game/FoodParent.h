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

class FoodParent : public engine::BaseComponent, engine::Observer<engine::Collider::TriggerType, engine::Collider*, engine::Collider*>
{
public:
	FoodParent(engine::GameObject* pGameObject);
	virtual ~FoodParent() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	void StartFall();
	void IncreaseFallExtraLevel() { m_FallExtraLevels++; }
	void DecreaseFallExtraLevel() { m_FallExtraLevels--; }

	virtual void Notify(engine::Collider::TriggerType triggerType, engine::Collider* pOriginCollider, engine::Collider* pHitCollider) override;

	bool IsFalling() { return m_IsFalling; }
	bool HasReachedPlate() { return m_ReachedPlate; }

	engine::Event<engine::GameObject*, bool>* GetFallEvent() { return m_FallEvent.get(); }
	engine::Event<FoodParent*>* GetReachedPlateEvent() { return m_ReachedPlateEvent.get(); }

private:
	void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerExit(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);
	void HandleTriggerStay(engine::Collider* pOriginCollider, engine::Collider* pHitCollider);

	void DropFoodElement(int elementId, bool skipDropLeftNeighbor, bool skipDropRightNeighbor);

	void StopFall(bool isForced);
	void ReachedPlate();

	//this would be done in a rigidbody component if we had one
	float m_FallVelocity{0};
	float m_FallAcceleration{-400};
	float m_BounceVelocity{ 50 };

	float m_YPosForFoodDown{-2}; //todo: maybe rename
	float m_MinYPosForFoodDown{-12};
	std::vector<engine::GameObject*> m_FoodElements{};
	std::vector<bool> m_FoodElementStates{}; //true == fallen

	std::unique_ptr<engine::Event<engine::GameObject*, bool>> m_FallEvent{};
	std::unique_ptr<engine::Event<FoodParent*>> m_ReachedPlateEvent{};

	bool m_IsFalling{};
	bool m_ReachedPlate{};

	int m_DropSound{};

	int m_FallExtraLevels{};
};

