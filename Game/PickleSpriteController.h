#pragma once
#include "EnemySpriteController.h"
class PickleSpriteController : public EnemySpriteController
{
public:
	PickleSpriteController(engine::GameObject* pGameObject) :EnemySpriteController(pGameObject) {};
	virtual ~PickleSpriteController() = default;

	virtual void Initialize() override;
};

