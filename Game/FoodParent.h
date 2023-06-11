#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "CollisionEventReceiver.h"
#include "Collider.h"
#include "Event.h"
#include "EventTypes.h"
#include <vector>

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

class FoodParent : public engine::BaseComponent, public engine::CollisionEventReceiver
{
public:
	FoodParent(engine::GameObject* pGameObject);
	virtual ~FoodParent() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	void StartFall();
	void EnemyEntered();
	void EnemyLeft();

	int GetEnemiesOnTopCount() { return m_EnemiesOnTop; }

	bool IsFalling() { return m_IsFalling; }
	bool HasReachedPlate() { return m_ReachedPlate; }

	engine::Event<engine::GameObject*, EventType>* GetFallEvent() { return m_FallEvent.get(); }
	engine::Event<FoodParent*>* GetReachedPlateEvent() { return m_ReachedPlateEvent.get(); }

private:
	virtual void HandleTriggerEnter(engine::Collider* pOriginCollider, engine::Collider* pHitCollider)override;

	void DropFoodElement(int elementId, bool skipDropLeftNeighbor, bool skipDropRightNeighbor);

	void StopFall(bool isForced);
	void ReachedPlate();

	//this would be done in a rigidbody component if we had one
	float m_FallVelocity{0};
	float m_FallAcceleration{-400};
	float m_BounceVelocity{ 50 };

	float m_YPosForFoodDown{-2};
	float m_MinYPosForFoodDown{-12};
	std::vector<engine::GameObject*> m_FoodElements{};
	std::vector<bool> m_FoodElementStates{}; //true == fallen

	std::unique_ptr<engine::Event<engine::GameObject*, EventType>> m_FallEvent{};
	std::unique_ptr<engine::Event<FoodParent*>> m_ReachedPlateEvent{};

	bool m_IsFalling{};
	bool m_ReachedPlate{};

	int m_DropSound{};
	int m_BounceSound{};

	int m_FallExtraLevels{};
	int m_EnemiesOnTop{}; //purely for scoring
};

