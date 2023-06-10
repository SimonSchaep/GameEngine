#pragma once
#include <BaseComponent.h>
#include <glm/glm.hpp>

class ThrowPepperComponent : public engine::BaseComponent
{
public:
	ThrowPepperComponent(engine::GameObject* pGameObject, int startingPepper) : engine::BaseComponent(pGameObject), m_PepperLeft{ startingPepper } {};

	virtual void Initialize() override {};
	virtual void Update() override;

	void ThrowPepper(const glm::vec2& direction);

	bool IsThrowing() { return m_ThrowDurationTimer > 0; }

	void AddPepper(int amount) { m_PepperLeft += amount; }

private:
	void CreatePepperObject(glm::vec2 direction);

	int m_PepperLeft{};

	float m_ThrowDistance{26};

	float m_ThrowCooldown{ 0.2f }; //for cooldown
	float m_ThrowCooldownTimer{};

	float m_ThrowDuration{0.2f}; //for not moving and animation
	float m_ThrowDurationTimer{};
};

