#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"

class Level;

class MovementComponent : public engine::BaseComponent
{
public:
	MovementComponent(engine::GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~MovementComponent() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	void Move(const glm::vec2& direction);

	void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; };

	const glm::vec2& GetCurrentMovementDirection()const { return m_CurrentMovementDirection; }

private:
	void CheckMoveX();
	void CheckMoveY();

	bool CanMoveUp();
	bool CanMoveDown();
	bool CanMoveRight();
	bool CanMoveLeft();

	void MoveAlongX();
	void MoveAlongY();

	float m_MoveSpeed{};
	glm::vec2 m_Direction{};

	glm::vec2 m_CurrentMovementDirection{}; //only used for animation, maybe there's a better way to do it, something like using velocity

	int m_CenterMargin{ 1 };

	Level* m_pLevel{};
};

