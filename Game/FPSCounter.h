#pragma once
#include "BaseComponent.h"


//todo: add avg fps


namespace engine
{
	class TextRenderComponent;
}

using namespace engine;

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