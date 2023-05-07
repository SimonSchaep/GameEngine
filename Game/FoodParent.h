#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include <vector>

namespace engine
{
	class GameObject;
	class Collider;
	class BoxCollider;
}

using namespace engine;

class FoodParent : public BaseComponent, Observer<Collider*, Collider*>
{
public:
	FoodParent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~FoodParent() = default;

	virtual void Initialize() override;
	virtual void Update() override {};

	virtual void Notify(Collider* pOriginCollider, Collider* pHitCollider) override;

private:
	float GetLowestYPosOfNeighbors(int foodElementIndex);

	float m_FallSpeed{50};
	float m_YLowerPerFoodDown{2};
	std::vector<GameObject*> m_FoodElements{};
	std::vector<bool> m_FoodElementStates{}; //true == fallen
};

