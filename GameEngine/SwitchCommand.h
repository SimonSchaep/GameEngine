#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class BaseComponent;

class SwitchCommand : public BaseCommand
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