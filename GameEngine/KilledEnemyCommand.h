#pragma once
#include "BaseCommand.h"
#include "MyPlayerController.h"

class KilledEnemyCommand : public BaseCommand
{
public:
	KilledEnemyCommand(MyPlayerController* pMovementController)
		: m_pMovementController{ pMovementController }
	{
	}

	virtual ~KilledEnemyCommand() = default;

	virtual void Execute() override
	{
		m_pMyPlayerController->KilledEnemy();
	}

private:
	MyPlayerController* m_pMyPlayerController;
};