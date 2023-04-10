#pragma once
#include "BaseComponent.h"
#include "Observer.h"
class DieEvent;
class ResetLivesEvent;

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

	DieEvent* GetDieEvent() { return m_DieEvent.get(); }
	ResetLivesEvent* GetResetLivesEvent() { return m_ResetLivesEvent.get(); }

private:
	int m_MaxLives{3};
	int m_Lives{};
	std::unique_ptr<DieEvent> m_DieEvent{};
	std::unique_ptr<ResetLivesEvent> m_ResetLivesEvent{};

};

