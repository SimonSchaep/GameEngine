#pragma once
#include "BaseComponent.h"

using namespace engine;

class DestroyComponent : public BaseComponent
{
public:
	DestroyComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~DestroyComponent() = default;

	virtual void Initialize() override {};
	virtual void Update() override;

	void SetLifeTime(float lifeTime) { m_LifeTime = lifeTime; };
	void SetDestroyChildren(bool destroyChildren) { m_DestroyChildren = destroyChildren; };

private:
	float m_LifeTime{5};
	bool m_DestroyChildren{};

};

