#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "ObservingPointer.h"

namespace engine
{
	class SpriteStateMachineComponent;
}

class ChefSpriteController : public engine::BaseComponent, engine::Observer<>
{
public:
	ChefSpriteController(engine::GameObject* pGameObject);
	virtual ~ChefSpriteController() = default;

	virtual void Initialize() override;
	virtual void Update() override {};

	virtual void Notify()override;

private:
	engine::ObservingPointer<engine::SpriteStateMachineComponent> m_pSpriteStateMachine;

	bool m_HasPlayerWon{};
};