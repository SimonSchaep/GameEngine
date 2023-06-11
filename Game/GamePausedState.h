#pragma once
#include "GameState.h"
#include "Observer.h"
#include <vector>
#include "EventTypes.h"

namespace engine
{
	class GameObject;
	class BaseCommand;
}

class GamePausedState : public GameState
{
public:
	GamePausedState(GameManager* pGameManager);

	void ResumeGame() { m_ResumeGame = true; }

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;
private:
	engine::GameObject* m_pPauseMenuGameObject{};
	bool m_ResumeGame{};
	std::vector<engine::BaseCommand*> m_Commands{};
};