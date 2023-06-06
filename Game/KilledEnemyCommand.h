#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"
#include "ObservingPointer.h"

class KilledEnemyCommand : public engine::BaseCommand
{
public:
	KilledEnemyCommand(MyPlayerController* pMyPlayerController)
		: m_pMyPlayerController{ pMyPlayerController }
	{
	}

	virtual ~KilledEnemyCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pMyPlayerController)return false;
		m_pMyPlayerController->KilledEnemy();
		return true;
	}

private:
	engine::ObservingPointer<MyPlayerController> m_pMyPlayerController;
};