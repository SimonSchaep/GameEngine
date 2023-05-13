#pragma once
#include "BaseComponent.h"

class DestroyComponent : public engine::BaseComponent
{
public:
	DestroyComponent(engine::GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~DestroyComponent() = default;

	virtual void Initialize() override {};
	virtual void Update() override;

	void SetLifeTime(float lifeTime) { m_LifeTime = lifeTime; };
	void SetDestroyChildren(bool destroyChildren) { m_DestroyChildren = destroyChildren; };

private:
	float m_LifeTime{5};
	bool m_DestroyChildren{};

};

