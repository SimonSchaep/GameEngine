#pragma once
#include "BaseComponent.h"
#include "ObservingPointer.h"


//todo: add avg fps


namespace engine
{
	class UITextRenderComponent;
}

class FPSCounter : public engine::BaseComponent
{
public:
	FPSCounter(engine::GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~FPSCounter() = default;

	virtual void Initialize() override;
	virtual void Update() override;

private:
	engine::ObservingPointer<engine::UITextRenderComponent> m_UITextRenderComponent{};
};