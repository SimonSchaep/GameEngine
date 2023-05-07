#include "MovementComponent.h"
#include "MoveCommands.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "InputController.h"
#include "PlayerController.h"
#include "Level.h"
#include <iostream>

//todo: fix jittering when moving left/right when we can't


void MovementComponent::Update()
{
	if (m_Direction != glm::vec2{0, 0})
	{
		//normalize if length more than 1
		float magnitude = glm::length(m_Direction);
		if (magnitude > 1)
		{
			m_Direction /= magnitude;
		}

		if (abs(m_Direction.x) >= abs(m_Direction.y))
		{
			CheckMoveX();
			CheckMoveY();
		}
		else //change order
		{
			CheckMoveY();
			CheckMoveX();
		}
		
				

		m_Direction = { 0,0 };
	}
}

void MovementComponent::Move(const glm::vec2& direction)
{
	m_Direction += direction;
}

void MovementComponent::CheckMoveX()
{
	Level& level = Level::GetInstance();

	auto transform = GetGameObject()->GetTransform();
	int row{};
	int col{};
	level.GetRowColOfPos(transform->GetWorldPosition(), row, col);

	const float epsilon = 0.0001f;

	if (abs(m_Direction.x) > epsilon) //if there is x input
	{
		//try move x
		if ((m_Direction.x > epsilon && CanMoveRight()) ||
			(m_Direction.x < -epsilon && CanMoveLeft()))
		{
			MoveAlongX();
		}
		else
		{
			if (abs(m_Direction.y) <= epsilon && !level.IsInCenterOfElementY(transform->GetWorldPosition(), m_CenterMargin)) //if there is no y input -> try to move closer to center
			{
				if (transform->GetWorldPosition().y > level.GetCenterOfCell(row, col).y)
				{
					m_Direction.y = -1;
				}
				else
				{
					m_Direction.y = 1;
				}
				if ((m_Direction.y > epsilon && CanMoveUp()) ||
					(m_Direction.y < -epsilon && CanMoveDown()))
				{
					MoveAlongY();
				}
			}
		}
	}
}

void MovementComponent::CheckMoveY()
{
	Level& level = Level::GetInstance();

	auto transform = GetGameObject()->GetTransform();
	int row{};
	int col{};
	level.GetRowColOfPos(transform->GetWorldPosition(), row, col);

	const float epsilon = 0.0001f;

	if (abs(m_Direction.y) > epsilon) //if there is y input
	{
		//try move y
		if ((m_Direction.y > epsilon && CanMoveUp()) ||
			(m_Direction.y < -epsilon && CanMoveDown()))
		{
			MoveAlongY();
		}
		else
		{
			if (abs(m_Direction.x) <= epsilon && !level.IsInCenterOfElementX(transform->GetWorldPosition(), m_CenterMargin)) //if there is no x input -> try to move closer to center
			{
				if (transform->GetWorldPosition().x > level.GetCenterOfCell(row, col).x)
				{
					m_Direction.x = -1;
				}
				else
				{
					m_Direction.x = 1;
				}
				if ((m_Direction.x > epsilon && CanMoveRight()) ||
					(m_Direction.x < -epsilon && CanMoveLeft()))
				{
					MoveAlongX();
				}
			}
		}
	}
}

bool MovementComponent::CanMoveUp()
{
	Level& level = Level::GetInstance();

	auto transform = GetGameObject()->GetTransform();
	int row{};
	int col{};
	level.GetRowColOfPos(transform->GetWorldPosition(), row, col);

	if (transform->GetWorldPosition().y < level.GetCenterOfCell(row, col).y)
	{
		return true;
	}
	return level.IsInCenterOfElementX(transform->GetWorldPosition(), m_CenterMargin)
		&& level.IsNavigableByPlayer(row + 1, col);
}

bool MovementComponent::CanMoveDown()
{
	Level& level = Level::GetInstance();

	auto transform = GetGameObject()->GetTransform();
	int row{};
	int col{};
	level.GetRowColOfPos(transform->GetWorldPosition(), row, col);

	if (transform->GetWorldPosition().y > level.GetCenterOfCell(row, col).y)
	{
		return true;
	}
	return level.IsInCenterOfElementX(transform->GetWorldPosition(), m_CenterMargin)
		&& level.IsNavigableByPlayer(row - 1, col);
}

bool MovementComponent::CanMoveRight()
{
	Level& level = Level::GetInstance();

	auto transform = GetGameObject()->GetTransform();
	int row{};
	int col{};
	level.GetRowColOfPos(transform->GetWorldPosition(), row, col);

	if (transform->GetWorldPosition().x < level.GetCenterOfCell(row, col).x)
	{
		return true;
	}
	return level.IsInCenterOfElementY(transform->GetWorldPosition(), m_CenterMargin)
		&& level.IsNavigableByPlayer(row, col + 1);
}

bool MovementComponent::CanMoveLeft()
{
	Level& level = Level::GetInstance();

	auto transform = GetGameObject()->GetTransform();
	int row{};
	int col{};
	level.GetRowColOfPos(transform->GetWorldPosition(), row, col);

	if (transform->GetWorldPosition().x > level.GetCenterOfCell(row, col).x)
	{
		return true;
	}
	return level.IsInCenterOfElementY(transform->GetWorldPosition(), m_CenterMargin)
		&& level.IsNavigableByPlayer(row, col - 1);
}

void MovementComponent::MoveAlongX()
{
	auto transform = GetGameObject()->GetTransform();
	transform->Translate(glm::normalize(glm::vec2{ m_Direction.x, 0 }) * m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime());
	glm::vec2 centerYPos = transform->GetWorldPosition();
	Level::GetInstance().SnapToCenterY(centerYPos);
	transform->SetWorldPosition(centerYPos);

	m_Direction = { 0,0 }; //make sure there is no further movement
}

void MovementComponent::MoveAlongY()
{
	auto transform = GetGameObject()->GetTransform();
	transform->Translate(glm::normalize(glm::vec2{ 0, m_Direction.y }) * m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime());
	glm::vec2 centerXPos = transform->GetWorldPosition();
	Level::GetInstance().SnapToCenterX(centerXPos);
	transform->SetWorldPosition(centerXPos);

	m_Direction = { 0,0 }; //make sure there is no further movement
}


