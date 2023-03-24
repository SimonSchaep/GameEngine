#pragma once
#include "BaseCommand.h"
#include "PlayerMovementController.h"

class BaseComponent;

class SwitchCommand : public BaseCommand
{
public:
	SwitchCommand(BaseComponent* pComponent) : BaseCommand(pComponent) {};

	virtual void Execute() override
	{
		static_cast<PlayerMovementController*>(GetComponent())->SwitchControlledGameObjects();
	}
};