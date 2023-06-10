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

enum class GameMode;

class StartMenuState : public GameState
{
public:
	StartMenuState(GameManager* pGameManager, engine::GameObject* pMenuGameObject);

	void StartGame(GameMode gameMode);

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;
private:
	engine::GameObject* m_pMenuGameObject{};
	bool m_StartGame{};
	GameMode m_GameMode{};
	std::vector<engine::BaseCommand*> m_Commands{};
};