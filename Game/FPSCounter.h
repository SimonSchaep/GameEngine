#pragma once
#include "BaseComponent.h"


//todo: add avg fps


namespace engine
{
	class TextRenderComponent;
}

class FPSCounter : public engine::BaseComponent
{
public:
	FPSCounter(engine::GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~FPSCounter() = default;

	virtual void Initialize() override;
	virtual void Update() override;

private:
	engine::TextRenderComponent* m_pTextRenderComponent{};
};