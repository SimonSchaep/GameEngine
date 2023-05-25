#pragma once
#include <BaseComponent.h>
#include <vector>

class GameState;
class StartMenuState;
class GamePlayingState;
class GamePausedState;
class LeaderboardState;

class GameManager final : public engine::BaseComponent
{
public:
	GameManager(engine::GameObject* pGameObject);
	virtual ~GameManager();

	virtual void Initialize() override;
	virtual void Update() override;

	StartMenuState* GetStartMenuState()const { return m_StartMenuState.get(); }
	GamePlayingState* GetGamePlayingState()const { return m_GamePlayingState.get(); }
	GamePausedState* GetGamePausedState()const { return m_GamePausedState.get(); }
	LeaderboardState* GetLeaderboardState()const { return m_LeaderboardState.get(); }

private:
	std::unique_ptr<StartMenuState> m_StartMenuState{};
	std::unique_ptr<GamePlayingState> m_GamePlayingState{};
	std::unique_ptr<GamePausedState> m_GamePausedState{};
	std::unique_ptr<LeaderboardState> m_LeaderboardState{};

	GameState* m_pActiveGameState{};
};
