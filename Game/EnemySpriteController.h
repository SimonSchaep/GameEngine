#pragma once
#include <BaseComponent.h>
#include "Observer.h"
#include "ObservingPointer.h"
#include <glm/glm.hpp>
#include <string>

namespace engine
{
	class SpriteStateMachineComponent;
	class SpriteState;
}

class MovementComponent;

class EnemySpriteController : public engine::BaseComponent, engine::Observer<>
{
public:
	EnemySpriteController(engine::GameObject* pGameObject);
	virtual ~EnemySpriteController() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void Notify()override;

protected:
	void SetRunningSpriteName(const std::string& runningSpriteName) { m_RunningSpriteName = runningSpriteName; };
	void SetClimbingDownSpriteName(const std::string& climbingDownSpriteName) { m_ClimbingDownSpriteName = climbingDownSpriteName; };
	void SetClimbingUpSpriteName(const std::string& climbingUpSpriteName) { m_ClimbingUpSpriteName = climbingUpSpriteName; };
	void SetDeathSpriteName(const std::string& deathSpriteName) { m_DeathSpriteName = deathSpriteName; };
	void SetStunnedSpriteName(const std::string& stunnedSpriteName) { m_StunnedSpriteName = stunnedSpriteName; };

private:
	void AddStateToLeft(engine::SpriteState* state, engine::SpriteState* runningState);
	void AddStateToRight(engine::SpriteState* state, engine::SpriteState* runningState);
	void AddStateToUp(engine::SpriteState* state, engine::SpriteState* runningState);
	void AddStateToDown(engine::SpriteState* state, engine::SpriteState* runningState);

	void AddStateToDeath(engine::SpriteState* state, engine::SpriteState* deathState);
	void AddStateToStunned(engine::SpriteState* state, engine::SpriteState* stunnedState);

	std::string m_RunningSpriteName{};
	std::string m_ClimbingDownSpriteName{};
	std::string m_ClimbingUpSpriteName{};
	std::string m_DeathSpriteName{};
	std::string m_StunnedSpriteName{};

	engine::ObservingPointer<engine::SpriteStateMachineComponent> m_pSpriteStateMachine;
	engine::ObservingPointer<MovementComponent> m_pMovementComponent;

	bool m_IsDead{};
	bool m_IsStunned{};

};