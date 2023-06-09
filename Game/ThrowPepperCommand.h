#pragma once
#include "BaseCommand.h"
#include "ChefPlayerController.h"
#include "ObservingPointer.h"

class ThrowPepperCommand : public engine::BaseCommand
{
public:
	ThrowPepperCommand(ChefPlayerController* pChefPlayerController)
		: m_pChefPlayerController{ pChefPlayerController }
	{
	}

	virtual ~ThrowPepperCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pChefPlayerController)return false;
		m_pChefPlayerController->ThrowPepper();
		return true;
	}

private:
	engine::ObservingPointer<ChefPlayerController> m_pChefPlayerController;
};