#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"
#include "ObservingPointer.h"

class ResetLivesCommand : public engine::BaseCommand
{
public:
	ResetLivesCommand(MyPlayerController* pMyPlayerController)
		:m_pMyPlayerController{ pMyPlayerController }
	{}

	virtual ~ResetLivesCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pMyPlayerController)return false;
		m_pMyPlayerController->ResetLives(); //todo: this doesn't make sense
		return true;
	}

private:
	engine::ObservingPointer<MyPlayerController> m_pMyPlayerController;
};