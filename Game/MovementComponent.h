#pragma once
#include "BaseComponent.h"
#include "ObservingPointer.h"
#include "glm/glm.hpp"

class Level;

class MovementComponent : public engine::BaseComponent
{
public:
	MovementComponent(engine::GameObject* pGameObject);
	virtual ~MovementComponent();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void OnSceneTransferred()override;

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

	engine::ObservingPointer<Level> m_pLevel;
};

