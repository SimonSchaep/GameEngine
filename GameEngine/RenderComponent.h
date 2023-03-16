#pragma once
#include "BaseComponent.h"

class RenderComponent : public BaseComponent
{
public:
	RenderComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~RenderComponent() = default;

	virtual void Initialize() override {};

	virtual void Update() override {};

	virtual void Render()const {};
};

