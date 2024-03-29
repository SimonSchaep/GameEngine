#pragma once
#include "GameState.h"
#include "Observer.h"
#include "EventTypes.h"
#include <vector>

namespace engine
{
	class GameObject;
	class BaseCommand;
}

class GamePausedState : public GameState
{
public:
	GamePausedState(GameManager* pGameManager);

	void BackToMenu() { m_BackToMenu = true; }
	void ResumeGame() { m_ResumeGame = true; }

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;
private:
	engine::GameObject* m_pPauseMenuGameObject{};
	bool m_BackToMenu{};
	bool m_ResumeGame{};
	std::vector<engine::BaseCommand*> m_Commands{};
};