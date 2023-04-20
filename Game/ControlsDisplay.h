#pragma once
#include "UIRenderComponent.h"

using namespace engine;

class ControlsDisplay : public UIRenderComponent
{
public:
	ControlsDisplay(GameObject* pGameObject) : UIRenderComponent(pGameObject) {};
	virtual ~ControlsDisplay() = default;

	virtual void RenderUI() override;
};

