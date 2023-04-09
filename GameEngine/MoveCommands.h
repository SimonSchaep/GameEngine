#pragma once
#include "BaseCommand.h"
#include "BaseAxisCommand.h"
#include "PlayerMovementController.h"

class MoveCommand : public BaseCommand
{
public:
	MoveCommand(PlayerMovementController* pMovementController, const glm::vec2& direction)
		: m_pMovementController{ pMovementController } 
		, m_Direction{direction}
	{}

	virtual ~MoveCommand() = default;

	virtual void Execute() override
	{
		m_pMovementController->Move(m_Direction);
	}

private:
	PlayerMovementController* m_pMovementController{};
	glm::vec2 m_Direction{};
};



class MoveAxisCommand : public BaseAxisCommand
{
public:
	MoveAxisCommand(PlayerMovementController* pMovementController, const glm::vec2& direction)
		: m_pMovementController{ pMovementController } 
		, m_Direction{direction}
	{}

	virtual void Execute(float axisValue) override
	{
		m_pMovementController->Move(m_Direction * axisValue);
	}

private:
	PlayerMovementController* m_pMovementController{};
	glm::vec2 m_Direction{};
};
