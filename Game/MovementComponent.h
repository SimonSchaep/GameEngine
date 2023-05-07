#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"

using namespace engine;

class MovementComponent : public BaseComponent
{
public:
	MovementComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~MovementComponent() = default;

	virtual void Initialize() override {};
	virtual void Update() override;

	void Move(const glm::vec2& direction);

	void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; };

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

	int m_CenterMargin{ 1 };
};

