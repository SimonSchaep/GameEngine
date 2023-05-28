#pragma once
#include <memory>
#include <vector>

//todo: add going back to menu from pause and leaderboard
//add restarting game from pause menu


namespace engine
{
	class GameObject;
	class BaseCommand;
}

class GameManager;

class GameState
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
	GameManager* m_pGameManager{}; //todo: set this
};

//Start Menu
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

//Game Playing
class GamePlayingState : public GameState
{
public:
	GamePlayingState(GameManager* pGameManager);

	void PauseGame() { m_PauseGame = true; }
	void EndGame() { m_EndGame = true; }

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;

private:
	bool m_PauseGame{};
	bool m_EndGame{};
	std::vector<engine::BaseCommand*> m_Commands{};
};

//Game paused
class GamePausedState : public GameState
{
public:
	GamePausedState(GameManager* pGameManager, engine::GameObject* pPauseMenuGameObject);

	void ResumeGame() { m_ResumeGame = true; }

	virtual GameState* Update()override;
	virtual void OnEnter()override;
	virtual void OnExit()override;
private:
	engine::GameObject* m_pPauseMenuGameObject{};
	bool m_ResumeGame{};
	std::vector<engine::BaseCommand*> m_Commands{};
};

//Leaderboard
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