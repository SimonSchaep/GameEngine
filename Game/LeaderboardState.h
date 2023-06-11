#pragma once
#include "GameState.h"
#include "Observer.h"
#include "EventTypes.h"
#include <vector>
#include <string>

namespace engine
{
	class GameObject;
	class BaseCommand;
}

class NameInputBox;

class LeaderboardState : public GameState, public engine::Observer<EventType, const std::string&>
{
public:
	LeaderboardState(GameManager* pGameManager);

	void BackToMenu() { m_BackToMenu = true; }

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;

	virtual void Notify(EventType type, const std::string& name);
private:
	std::vector<engine::BaseCommand*> m_Commands{};

	engine::GameObject* m_pLeaderboardGameObject{};
	engine::GameObject* m_pHighScoresGameObject{};

	NameInputBox* m_pNameInputBox{};

	bool m_BackToMenu{};
};