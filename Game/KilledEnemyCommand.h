#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class KilledEnemyCommand : public engine::BaseCommand
{
public:
	KilledEnemyCommand(MyPlayerController* pMovementController)
		: m_pMyPlayerController{ pMovementController }
	{
	}

	virtual ~KilledEnemyCommand() = default;

	virtual bool Execute() override
	{
		m_pMyPlayerController->KilledEnemy();
		return true;
	}

private:
	MyPlayerController* m_pMyPlayerController;
};