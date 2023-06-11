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

	void AddLife() { m_LivesLeft++; }

	int GetPepperLeft() { return m_PepperLeft; }
	void AddPepper(int amount) { m_PepperLeft += amount; }

	void Reset();

	virtual void Notify(EventType type)override;

private:
	bool m_PauseGame{};
	bool m_EndGame{};
	std::vector<engine::BaseCommand*> m_Commands{};

	float m_SpawnSpecialPickupInterval{15.f};
	float m_SpawnSpecialPickupTimer{};

	int m_LivesLeft{3};
	int m_PepperLeft{ 5 }; //just holds the global count, doesn't do anything with it
};