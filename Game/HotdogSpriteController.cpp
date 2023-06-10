#include "HotdogSpriteController.h"

void HotdogSpriteController::Initialize()
{
	SetRunningSpriteName("hotdogRunning.png");
	SetClimbingUpSpriteName("hotdogClimbingUp.png");
	SetClimbingDownSpriteName("hotdogClimbingDown.png");
	SetDeathSpriteName("hotdogDeath.png");
	SetStunnedSpriteName("hotdogStunned.png");

	EnemySpriteController::Initialize();
}
