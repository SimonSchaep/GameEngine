#pragma once
#include "BaseComponent.h"
#include "Event.h"

//this would be part of some other component in the game


class PlayerPoints : public engine::BaseComponent
{
public:
	PlayerPoints(engine::GameObject* pGameObject);
	virtual ~PlayerPoints() = default;

	virtual void Initialize() override {};
	virtual void Update() override {};

	void KilledEnemy();

	engine::Event<>* GetKilledEnemyEvent() { return m_KilledEnemyEvent.get(); }

private:
	std::unique_ptr<engine::Event<>> m_KilledEnemyEvent{};
};

