#pragma once
#include "BaseButtonCommand.h"
#include "PlayerMovementController.h"

class BaseComponent;

class SwitchCommand : public BaseButtonCommand
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