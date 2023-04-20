#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class DieCommand : public engine::BaseCommand
{
public:
	DieCommand(MyPlayerController* pMovementController)
		:m_pMovementController{ pMovementController }
	{}

	virtual ~DieCommand() = default;

	virtual void Execute() override
	{
		m_pMovementController->Die();
	}

private:
	MyPlayerController* m_pMovementController;
};