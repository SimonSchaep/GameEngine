#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "ObservingPointer.h"
#include <glm/glm.hpp>
#include "EventTypes.h"

namespace engine
{
	class SpriteStateMachineComponent;
	class SpriteState;
}

class MovementComponent;
class ThrowPepperComponent;

class ChefSpriteController : public engine::BaseComponent, engine::Observer<EventType>
{
public:
	ChefSpriteController(engine::GameObject* pGameObject);
	virtual ~ChefSpriteController() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Notify(EventType eventType) override;

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

	bool m_HasPlayerWon{};
	bool m_IsPlayerDead{};
	bool m_IsPlayerThrowing{};
};