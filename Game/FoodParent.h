#pragma once
#include "BaseComponent.h"
#include <vector>

using namespace engine;

class FoodElement;

class FoodParent : public BaseComponent
{
public:
	FoodParent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~FoodParent() = default;

	virtual void Initialize() override {};
	virtual void Update() override {};

private:
	float m_FallSpeed{};
	std::vector<FoodElement*> m_FoodElements{};
};

