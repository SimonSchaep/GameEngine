#pragma once
#include "GameState.h"
#include "Observer.h"
#include <vector>
#include "EventTypes.h"

namespace engine
{
	class GameObject;
}

class LeaderboardState : public GameState
{
public:
	LeaderboardState(GameManager* pGameManager, engine::GameObject* pLeaderboardGameObject);

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;
private:
	engine::GameObject* m_pLeaderboardGameObject{};
};