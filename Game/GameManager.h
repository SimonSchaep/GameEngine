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
class ChefLogic;

enum class GameMode
{
	singleplayer,
	coop,
	versus,
};

class GameManager final : public engine::BaseComponent, public engine::Observer<FoodParent*>, public engine::Observer<EventType, ChefLogic*>
{
public:
	GameManager(engine::GameObject* pGameObject);
	virtual ~GameManager();

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void OnSceneTransferred()override;

	void StartNextLevel();
	void StartGame(GameMode gameMode);

	StartMenuState* GetStartMenuState()const { return m_StartMenuState.get(); }
	GamePlayingState* GetGamePlayingState()const { return m_GamePlayingState.get(); }
	GamePausedState* GetGamePausedState()const { return m_GamePausedState.get(); }
	LeaderboardState* GetLeaderboardState()const { return m_LeaderboardState.get(); }

	engine::Event<EventType>* GetOnChefWon()const { return m_OnChefWon.get(); }
	engine::Event<EventType, ChefLogic*>* GetOnChefDied()const { return m_OnChefDied.get(); }
	engine::Event<EventType>* GetOnRespawnCharacters()const { return m_OnRespawnCharacters.get(); }

	virtual void Notify(FoodParent* pFood)override;
	virtual void Notify(EventType, ChefLogic*)override;

private:
	void InitializeUI();
	engine::Scene* CreateLevel(int id);
	void AssignControllers();
	void CheckIfChefWon();

	ChefPlayerController* CreateChefPlayerController(engine::Scene* pScene);
	EnemyAIController* CreateEnemyAIController(engine::Scene* pScene);

	std::unique_ptr<StartMenuState> m_StartMenuState{};
	std::unique_ptr<GamePlayingState> m_GamePlayingState{};
	std::unique_ptr<GamePausedState> m_GamePausedState{};
	std::unique_ptr<LeaderboardState> m_LeaderboardState{};

	GameState* m_pActiveGameState{};

	engine::Scene* m_pNextLevel{};
	int m_NextLevelId{0};
	int m_MaxLevelId{3}; //exclusive

	std::unique_ptr<engine::Event<EventType>> m_OnChefWon{};
	std::unique_ptr<engine::Event<EventType, ChefLogic*>> m_OnChefDied{};
	std::unique_ptr<engine::Event<EventType>> m_OnRespawnCharacters{};

	bool m_FoodsNeedUpdate{};
	std::vector<engine::ObservingPointer<FoodParent>> m_FoodsLeftinLevel{};


	float m_RespawnCharactersDelay{3.f};
	float m_RespawnCharactersDelayTimer{};

	float m_StartNextLevelDelay{ 3.f };
	float m_StartNextLevelDelayTimer{};

	GameMode m_GameMode{GameMode::singleplayer};
};

