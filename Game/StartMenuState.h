#pragma once
#include "GameState.h"
#include <vector>

namespace engine
{
	class GameObject;
	class BaseCommand;
}

enum class GameMode;

class StartMenuState : public GameState
{
public:
	StartMenuState(GameManager* pGameManager);

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