#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class ResetLivesCommand : public BaseCommand
{
public:
	ResetLivesCommand(MyPlayerController* pMovementController)
		:m_pMovementController{ pMovementController }
	{}

	virtual ~ResetLivesCommand() = default;

	virtual void Execute() override
	{
		m_pMovementController->ResetLives();
	}

private:
	MyPlayerController* m_pMovementController;
};