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

	virtual bool Execute() override
	{
		if (!m_pMovementController)return false;
		m_pMovementController->Move(m_Direction);
		return true;
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

	virtual bool Execute(float axisValue) override
	{
		m_pMovementController->Move(m_Direction * axisValue);
		return true;
	}

private:
	MyPlayerController* m_pMovementController{};
	glm::vec2 m_Direction{};
};
