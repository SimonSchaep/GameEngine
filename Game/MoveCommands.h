#pragma once
#include "BaseCommand.h"
#include "BaseAxisCommand.h"
#include "MyPlayerController.h"

class MoveCommand : public engine::BaseCommand
{
public:
	MoveCommand(MyPlayerController* pMovementController, const glm::vec2& direction)
		: m_pMovementController{ pMovementController } 
		, m_Direction{direction}
	{}

	virtual ~MoveCommand() = default;

	virtual void Execute() override
	{
		m_pMovementController->Move(m_Direction);
	}

private:
	MyPlayerController* m_pMovementController{};
	glm::vec2 m_Direction{};
};



class MoveAxisCommand : public engine::BaseAxisCommand
{
public:
	MoveAxisCommand(MyPlayerController* pMovementController, const glm::vec2& direction)
		: m_pMovementController{ pMovementController } 
		, m_Direction{direction}
	{}

	virtual void Execute(float axisValue) override
	{
		m_pMovementController->Move(m_Direction * axisValue);
	}

private:
	MyPlayerController* m_pMovementController{};
	glm::vec2 m_Direction{};
};
