#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"
#include "ObservingPointer.h"

class SwitchCommand : public engine::BaseCommand
{
public:
	SwitchCommand(MyPlayerController* pMyPlayerController) : m_pMyPlayerController{ pMyPlayerController } {};

	virtual bool Execute() override
	{
		if (!m_pMyPlayerController)return false;
		m_pMyPlayerController->SwitchControlledGameObjects();
		return true;
	}

private:
	engine::ObservingPointer<MyPlayerController> m_pMyPlayerController;
};