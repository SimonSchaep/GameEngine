#pragma once
#include "BaseComponent.h"
#include "Event.h"

using namespace engine;

//this would be part of some other component in the game


class PlayerPoints : public BaseComponent
{
public:
	PlayerPoints(GameObject* pGameObject);
	virtual ~PlayerPoints() = default;

	virtual void Initialize() override {};
	virtual void Update() override {};

	void KilledEnemy();

	Event<>* GetKilledEnemyEvent() { return m_KilledEnemyEvent.get(); }

private:
	std::unique_ptr<Event<>> m_KilledEnemyEvent{};
};

