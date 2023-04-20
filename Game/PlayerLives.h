#pragma once
#include "BaseComponent.h"
#include "Observer.h"

using namespace engine;

class engine::Event;

class PlayerLives : public BaseComponent
{
public:
	PlayerLives(GameObject* pGameObject);
	virtual ~PlayerLives() = default;

	virtual void Initialize() override;
	virtual void Update() override {};

	void SetMaxLives(int count) { m_MaxLives = count; }
	int GetLives() { return m_Lives; }

	void Die();
	void ResetLives();

	Event* GetDieEvent() { return m_DieEvent.get(); }
	Event* GetResetLivesEvent() { return m_ResetLivesEvent.get(); }

private:
	int m_MaxLives{3};
	int m_Lives{};
	std::unique_ptr<Event> m_DieEvent{};
	std::unique_ptr<Event> m_ResetLivesEvent{};

};

