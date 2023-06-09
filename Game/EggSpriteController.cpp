#include "EggSpriteController.h"

void EggSpriteController::Initialize()
{
	SetRunningSpriteName("eggRunning.png");
	SetClimbingUpSpriteName("eggClimbingUp.png");
	SetClimbingDownSpriteName("eggClimbingDown.png");
	SetDeathSpriteName("eggDeath.png");
	SetStunnedSpriteName("eggStunned.png");

	EnemySpriteController::Initialize();
}
