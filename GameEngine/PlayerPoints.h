#pragma once
#include "BaseComponent.h"

class KilledEnemyEvent;

//this would be part of some other component in the game


class PlayerPoints : public BaseComponent
{
public:
	PlayerPoints(GameObject* pGameObject);
	virtual ~PlayerPoints() = default;

	virtual void Initialize() override {};
	virtual void Update() override {};

	void KilledEnemy();

	KilledEnemyEvent* GetKilledEnemyEvent() { return m_KilledEnemyEvent.get(); }

private:
	std::unique_ptr<KilledEnemyEvent> m_KilledEnemyEvent{};
};

