#include "BeanSpriteController.h"

void BeanSpriteController::Initialize()
{
	SetRunningSpriteName("beanRunning.png");
	SetClimbingUpSpriteName("beanClimbingUp.png");
	SetClimbingDownSpriteName("beanClimbingDown.png");
	SetDeathSpriteName("beanDeath.png");
	SetStunnedSpriteName("beanStunned.png");

	EnemySpriteController::Initialize();
}
