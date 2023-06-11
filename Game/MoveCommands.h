#pragma once
#include "BaseCommand.h"
#include "BaseAxisCommand.h"
#include "MyPlayerController.h"
#include "ObservingPointer.h"

class MoveCommand final : public engine::BaseCommand
{
public:
	MoveCommand(MyPlayerController* pMyPlayerController, const glm::vec2& direction)
		: m_pMyPlayerController{ pMyPlayerController }
		, m_Direction{direction}
	{}

	virtual ~MoveCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pMyPlayerController)return false;
		m_pMyPlayerController->Move(m_Direction);
		return true;
	}

private:
	engine::ObservingPointer<MyPlayerController> m_pMyPlayerController;
	glm::vec2 m_Direction{};
};



class MoveAxisCommand final : public engine::BaseAxisCommand
{
public:
	MoveAxisCommand(MyPlayerController* pMyPlayerController, const glm::vec2& direction)
		: m_pMyPlayerController{ pMyPlayerController }
		, m_Direction{direction}
	{}

	virtual bool Execute(float axisValue) override
	{
		if (!m_pMyPlayerController)return false;
		m_pMyPlayerController->Move(m_Direction * axisValue);
		return true;
	}

private:
	engine::ObservingPointer<MyPlayerController> m_pMyPlayerController;
	glm::vec2 m_Direction{};
};
