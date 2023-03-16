#pragma once
#include "BaseComponent.h"


//todo: add avg fps

class TextRenderComponent;

class FPSCounter : public BaseComponent
{
public:
	FPSCounter(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~FPSCounter() = default;

	virtual void Initialize() override;
	virtual void Update() override;

private:
	TextRenderComponent* m_pTextRenderComponent{};
};