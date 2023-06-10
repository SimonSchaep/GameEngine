#pragma once
#include "EnemySpriteController.h"
class HotdogSpriteController : public EnemySpriteController
{
public:
	HotdogSpriteController(engine::GameObject* pGameObject) :EnemySpriteController(pGameObject) {};
	virtual ~HotdogSpriteController() = default;

	virtual void Initialize() override;
};

