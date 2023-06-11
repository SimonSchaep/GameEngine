#pragma once
#include "EnemySpriteController.h"

class EggSpriteController final : public EnemySpriteController
{
public:
	EggSpriteController(engine::GameObject* pGameObject) :EnemySpriteController(pGameObject) {};
	virtual ~EggSpriteController() = default;

	virtual void Initialize() override;
};

