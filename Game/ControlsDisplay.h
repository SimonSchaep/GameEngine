#pragma once
#include "UIRenderComponent.h"

class ControlsDisplay final : public engine::UIRenderComponent
{
public:
	ControlsDisplay(engine::GameObject* pGameObject) : UIRenderComponent(pGameObject) {};
	virtual ~ControlsDisplay() = default;

	virtual void RenderUI() override;
};

