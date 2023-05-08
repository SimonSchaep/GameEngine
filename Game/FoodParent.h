#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "Collider.h"
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
	FoodParent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~FoodParent() = default;

	virtual void Initialize() override;
	virtual void Update() override {};

	void Fall();

	virtual void Notify(Collider::TriggerType triggerType, Collider* pOriginCollider, Collider* pHitCollider) override;

private:
	void HandleTriggerEnter(Collider* pOriginCollider, Collider* pHitCollider);
	void HandleTriggerExit(Collider* pOriginCollider, Collider* pHitCollider);
	void HandleTriggerStay(Collider* pOriginCollider, Collider* pHitCollider);

	float m_FallSpeed{50};
	float m_YPosForFoodDown{-4};
	std::vector<GameObject*> m_FoodElements{};
	std::vector<bool> m_FoodElementStates{}; //true == fallen

	bool m_HasFallen{};
};

