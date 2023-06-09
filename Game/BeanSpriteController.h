#pragma once
#include "EnemySpriteController.h"
class BeanSpriteController : public EnemySpriteController
{
public:
	BeanSpriteController(engine::GameObject* pGameObject) :EnemySpriteController(pGameObject) {};
	virtual ~BeanSpriteController() = default;

	virtual void Initialize() override;
};

