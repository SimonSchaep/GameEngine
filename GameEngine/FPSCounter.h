#pragma once
#include "BaseComponent.h"
#include <memory>

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