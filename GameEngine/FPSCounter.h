#pragma once
#include "BaseComponent.h"
#include <memory>


//todo: add delay to update

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