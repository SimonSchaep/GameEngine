#pragma once
#include "AIController.h"
#include "ObservingPointer.h"
#include "Observer.h"
#include "EventTypes.h"

class EnemyLogic;

class EnemyAIController final : public AIController
{
public:
	EnemyAIController(engine::GameObject* pGameObject, float spawnDelay);

	virtual void Initialize() override;

	virtual void Notify(EventType type);

	virtual void SetControlledObject(engine::GameObject* pControlledObject)override;

private:
	virtual void ProcessAIDecisions() override;

	bool HasReachedTargetTile();
	void FindNewTargetTile();

	struct AStarTile
	{
		int tile{};
		AStarTile* pPrevTile{};
		int cost{};
	};
	int FindTargetTileAStar();

	void UpdateTargets();
	void FindClosestTarget();

	std::vector<engine::ObservingPointer<engine::GameObject>> m_Targets{};
	engine::ObservingPointer<engine::GameObject> m_CurrentTarget{};
	engine::ObservingPointer<EnemyLogic> m_EnemyLogic{};

	int m_LastTile{};
	int m_TargetTile{};

	engine::ObservingPointer<Level> m_pLevel;

	bool m_TargetTileIsInvalid{};


	float m_SpawnDelay{};
	float m_SpawnDelayTimer{};
};

