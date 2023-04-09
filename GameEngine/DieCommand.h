#pragma once
#include "BaseButtonCommand.h"

class DieCommand : public BaseCommand
{
public:
	DieCommand()
		
	{
	}

	virtual ~DieCommand() = default;

	virtual void Execute() override
	{
		//send event
	}

private:
	//gameobject*
};