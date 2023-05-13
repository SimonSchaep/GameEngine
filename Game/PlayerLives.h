#pragma once
#include "BaseComponent.h"
#include "Event.h"

class PlayerLives : public engine::BaseComponent
{
public:
	PlayerLives(engine::GameObject* pGameObject);
	virtual ~PlayerLives() = default;

	virtual void Initialize() override;
	virtual void Update() override {};

	void SetMaxLives(int count) { m_MaxLives = count; }
	int GetLives() { return m_Lives; }

	void Die();
	void ResetLives();

	engine::Event<>* GetDieEvent() { return m_DieEvent.get(); }
	engine::Event<>* GetResetLivesEvent() { return m_ResetLivesEvent.get(); }

private:
	int m_MaxLives{3};
	int m_Lives{};
	std::unique_ptr<engine::Event<>> m_DieEvent{};
	std::unique_ptr<engine::Event<>> m_ResetLivesEvent{};

};

