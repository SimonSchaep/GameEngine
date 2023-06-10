#pragma once
#include <memory>
#include "ObservableObject.h"

//todo: add going back to menu from pause and leaderboard
//add restarting game from pause menu

class GameManager;

class GameState : public virtual engine::ObservableObject
{
public:
	GameState(GameManager* pGameManager) :m_pGameManager{ pGameManager } {};
	virtual ~GameState() = default;
	virtual GameState* Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

protected:
	GameManager* GetGameManager()const { return m_pGameManager; }

private:	
	GameManager* m_pGameManager{};
};
