#pragma once
#include "BaseComponent.h"

class LifeTimer final : public engine::BaseComponent
{
public:
	LifeTimer(engine::GameObject* pGameObject, float lifeTime) : engine::BaseComponent(pGameObject), m_LifeTimer{ lifeTime } {};

	virtual void Initialize() override {};
	virtual void Update() override;

	void SetLifeTime(float lifeTime) { m_LifeTimer = lifeTime; }

private:
	float m_LifeTimer{};

};

