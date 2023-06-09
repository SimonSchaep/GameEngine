#include "PickleSpriteController.h"

void PickleSpriteController::Initialize()
{
	SetRunningSpriteName("pickleRunning.png");
	SetClimbingUpSpriteName("pickleClimbingUp.png");
	SetClimbingDownSpriteName("pickleClimbingDown.png");
	SetDeathSpriteName("pickleDeath.png");
	SetStunnedSpriteName("pickleStunned.png");

	EnemySpriteController::Initialize();
}
