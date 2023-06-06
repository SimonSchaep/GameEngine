#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"
#include "ObservingPointer.h"

class DieCommand : public engine::BaseCommand
{
public:
	DieCommand(MyPlayerController* pMyPlayerController)
		:m_pMyPlayerController{ pMyPlayerController }
	{}

	virtual ~DieCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pMyPlayerController)return false;
		m_pMyPlayerController->Die();
		return true;
	}

private:
	engine::ObservingPointer<MyPlayerController> m_pMyPlayerController;
};