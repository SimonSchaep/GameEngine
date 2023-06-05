#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class SwitchCommand : public engine::BaseCommand
{
public:
	SwitchCommand(MyPlayerController* pMovementController) : m_pMovementController{ pMovementController } {};

	virtual bool Execute() override
	{
		m_pMovementController->SwitchControlledGameObjects();
		return true;
	}

private:
	MyPlayerController* m_pMovementController{};
};