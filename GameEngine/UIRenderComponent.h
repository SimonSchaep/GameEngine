#pragma once
#include "BaseComponent.h"
class UIRenderComponent : public BaseComponent
{
public:
	UIRenderComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~UIRenderComponent() = default;

	virtual void Initialize() override {};

	virtual void Update() override {};

	virtual void RenderUI() {};
};

