#pragma once
#include "BaseComponent.h"

class MoveComponent : public BaseComponent
{
public:
	MoveComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~MoveComponent() = default;

	virtual void Initialize() override;
	virtual void Update() override;

private:

};

