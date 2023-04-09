#pragma once
#include "BaseCommand.h"
#include "PlayerMovementController.h"

class BaseComponent;

class SwitchCommand : public BaseCommand
{
public:
	SwitchCommand(PlayerMovementController* pMovementController) : m_pMovementController{ pMovementController } {};

	virtual void Execute() override
	{
		m_pMovementController->SwitchControlledGameObjects();
	}

private:
	PlayerMovementController* m_pMovementController{};
};