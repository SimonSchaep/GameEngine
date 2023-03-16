#pragma once
#include "RenderComponent.h"

class Exercise1Component : public RenderComponent
{
public:
	Exercise1Component(GameObject* pGameObject) :RenderComponent(pGameObject) {};
	virtual ~Exercise1Component() = default;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render()const override;

private:

};

