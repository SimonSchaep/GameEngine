#pragma once
#include "BaseComponent.h"
#include "ObservingPointer.h"
#include <glm/glm.hpp>

class GamePlayingState;

class ThrowPepperComponent : public engine::BaseComponent
{
public:
	ThrowPepperComponent(engine::GameObject* pGameObject) : engine::BaseComponent(pGameObject) {};

	virtual void Initialize() override;
	virtual void Update() override;

	void ThrowPepper(const glm::vec2& direction);

	void AddPepper(int amount);

	bool IsThrowing() { return m_ThrowDurationTimer > 0; }

private:
	void CreatePepperObject(glm::vec2 direction);

	float m_ThrowDistance{26};

	float m_ThrowCooldown{ 0.2f }; //for cooldown
	float m_ThrowCooldownTimer{};

	float m_ThrowDuration{0.2f}; //for not moving and animation
	float m_ThrowDurationTimer{};

	engine::ObservingPointer<GamePlayingState> m_GamePlayingState{};
};

