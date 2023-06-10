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

class StartMenuState : public GameState
{
public:
	StartMenuState(GameManager* pGameManager, engine::GameObject* pMenuGameObject);

	void StartGame() { m_StartGame = true; }

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;
private:
	engine::GameObject* m_pMenuGameObject{};
	bool m_StartGame{};
	std::vector<engine::BaseCommand*> m_Commands{};
};