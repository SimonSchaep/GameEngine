#pragma once
#include "EnemySpriteController.h"

class HotdogSpriteController final : public EnemySpriteController
{
public:
	HotdogSpriteController(engine::GameObject* pGameObject) :EnemySpriteController(pGameObject) {};
	virtual ~HotdogSpriteController() = default;

	virtual void Initialize() override;
};

