#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class AddPointsCommand : public BaseCommand
{
public:
	AddPointsCommand(MyPlayerController* pMovementController, int amount)
		: m_pMovementController{ pMovementController }
		, m_Amount{amount}
	{
	}

	virtual ~AddPointsCommand() = default;

	virtual void Execute() override
	{
		m_pMovementController->AddPoints(m_Amount);
	}

private:
	MyPlayerController* m_pMovementController;
	int m_Amount;
};