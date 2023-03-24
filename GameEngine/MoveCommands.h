#pragma once
#include "BaseCommand.h"
#include "PlayerMovementController.h"

class BaseComponent;

class MoveLeftCommand : public BaseCommand
{
public:
	MoveLeftCommand(BaseComponent* pComponent) : BaseCommand(pComponent) {};

	virtual void Execute() override
	{
		static_cast<PlayerMovementController*>(GetComponent())->Move({ -1,0 });
	}
};

class MoveRightCommand : public BaseCommand
{
public:
	MoveRightCommand(BaseComponent* pComponent) : BaseCommand(pComponent) {};

	virtual void Execute() override
	{
		static_cast<PlayerMovementController*>(GetComponent())->Move({ 1,0 });
	}
};

class MoveUpCommand : public BaseCommand
{
public:
	MoveUpCommand(BaseComponent* pComponent) : BaseCommand(pComponent) {};

	virtual void Execute() override
	{
		static_cast<PlayerMovementController*>(GetComponent())->Move({ 0,-1 });
	}
};

class MoveDownCommand : public BaseCommand
{
public:
	MoveDownCommand(BaseComponent* pComponent) : BaseCommand(pComponent) {};

	virtual void Execute() override
	{
		static_cast<PlayerMovementController*>(GetComponent())->Move({ 0,1 });
	}
};

