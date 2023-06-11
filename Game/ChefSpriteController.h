#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "ObservingPointer.h"
#include "EventTypes.h"
#include <glm/glm.hpp>

namespace engine
{
	class SpriteStateMachineComponent;
	class SpriteState;
}

class MovementComponent;
class ThrowPepperComponent;
class ChefLogic;

class ChefSpriteController final : public engine::BaseComponent, public engine::Observer<EventType>, public engine::Observer<EventType, ChefLogic*>
{
public:
	ChefSpriteController(engine::GameObject* pGameObject);
	virtual ~ChefSpriteController() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Notify(EventType eventType) override;
	virtual void Notify(EventType eventType, ChefLogic*) override;

private:
	void AddIdleToLeft(engine::SpriteState* idleState, engine::SpriteState* runningState);
	void AddIdleToRight(engine::SpriteState* idleState, engine::SpriteState* runningState);
	void AddIdleToUp(engine::SpriteState* idleState, engine::SpriteState* runningState);
	void AddIdleToDown(engine::SpriteState* idleState, engine::SpriteState* runningState);

	void AddStateToThrow(engine::SpriteState* state, engine::SpriteState* throwState);

	void AddStateToWinning(engine::SpriteState* state, engine::SpriteState* winningState);
	void AddStateToDeath(engine::SpriteState* state, engine::SpriteState* deathState);

	engine::ObservingPointer<engine::SpriteStateMachineComponent> m_pSpriteStateMachine;

	engine::ObservingPointer<MovementComponent> m_pMovementComponent;
	engine::ObservingPointer<ThrowPepperComponent> m_pThrowPepperComponent;
	engine::ObservingPointer<ChefLogic> m_ChefLogicComponent;

	bool m_HasPlayerWon{};
};