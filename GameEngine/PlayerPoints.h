#pragma once
#include "BaseComponent.h"

class AddPointsEvent;

class PlayerPoints : public BaseComponent
{
public:
	PlayerPoints(GameObject* pGameObject);
	virtual ~PlayerPoints() = default;

	virtual void Initialize() override {};
	virtual void Update() override {};

	int GetPoints() { return m_Points; }

	void AddPoints(int amount);

	AddPointsEvent* GetAddPointsEvent() { return m_AddPointsEvent.get(); }

private:
	int m_Points{};
	std::unique_ptr<AddPointsEvent> m_AddPointsEvent{};
};

