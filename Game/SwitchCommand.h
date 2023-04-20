#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class SwitchCommand : public engine::BaseCommand
{
public:
	SwitchCommand(MyPlayerController* pMovementController) : m_pMovementController{ pMovementController } {};

	virtual void Execute() override
	{
		m_pMovementController->SwitchControlledGameObjects();
	}

private:
	MyPlayerController* m_pMovementController{};
};