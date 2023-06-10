#pragma once
#include <BaseComponent.h>
#include <vector>
#include "Event.h"
#include "EventTypes.h"
#include "Observer.h"

namespace engine
{
	class Scene;
}

class GameState;
class StartMenuState;
class GamePlayingState;
class GamePausedState;
class LeaderboardState;

class MyPlayerController;
class ChefPlayerController;
class AIController;
class EnemyAIController;

class FoodParent;

class GameManager final : public engine::BaseComponent, public engine::Observer<FoodParent*>
{
public:
	GameManager(engine::GameObject* pGameObject);
	virtual ~GameManager();

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void OnSceneTransferred()override;

	void StartNextLevel();

	StartMenuState* GetStartMenuState()const { return m_StartMenuState.get(); }
	GamePlayingState* GetGamePlayingState()const { return m_GamePlayingState.get(); }
	GamePausedState* GetGamePausedState()const { return m_GamePausedState.get(); }
	LeaderboardState* GetLeaderboardState()const { return m_LeaderboardState.get(); }

	engine::Event<EventType>* GetOnChefWon()const { return m_OnChefWon.get(); }

	virtual void Notify(FoodParent* pFood)override;

private:
	void InitializeUI();
	engine::Scene* CreateLevel(int id);
	void CheckIfChefWon();

	engine::GameObject* CreateChef(engine::Scene* pScene);
	engine::GameObject* CreateHotdog(engine::Scene* pScene);
	ChefPlayerController* CreateChefPlayerController(engine::Scene* pScene);
	EnemyAIController* CreateEnemyAIController(engine::Scene* pScene);

	std::unique_ptr<StartMenuState> m_StartMenuState{};
	std::unique_ptr<GamePlayingState> m_GamePlayingState{};
	std::unique_ptr<GamePausedState> m_GamePausedState{};
	std::unique_ptr<LeaderboardState> m_LeaderboardState{};

	GameState* m_pActiveGameState{};

	int m_NextLevelId{};
	int m_MaxLevelId{2};

	std::unique_ptr<engine::Event<EventType>> m_OnChefWon{};

	bool m_FoodsNeedUpdate{};
	std::vector<FoodParent*> m_FoodsLeftinLevel{};
};

