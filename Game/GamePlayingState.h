#pragma once
#include "GameState.h"
#include "Observer.h"
#include <vector>
#include "EventTypes.h"

namespace engine
{
	class BaseCommand;
}

class GamePlayingState : public GameState, public virtual engine::Observer<EventType>
{
public:
	GamePlayingState(GameManager* pGameManager);

	void PauseGame() { m_PauseGame = true; }
	void EndGame() { m_EndGame = true; }

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;

	virtual void Notify(EventType type)override;

private:
	bool m_PauseGame{};
	bool m_EndGame{};
	std::vector<engine::BaseCommand*> m_Commands{};

	float m_SpawnSpecialPickupInterval{15.f};
	float m_SpawnSpecialPickupTimer{};

	int m_LivesLeft{3};
};