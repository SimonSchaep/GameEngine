#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class ResetLivesCommand : public engine::BaseCommand
{
public:
	ResetLivesCommand(MyPlayerController* pMovementController)
		:m_pMovementController{ pMovementController }
	{}

	virtual ~ResetLivesCommand() = default;

	virtual bool Execute() override
	{
		m_pMovementController->ResetLives(); //todo: this doesn't make sense
		return true;
	}

private:
	MyPlayerController* m_pMovementController;
};