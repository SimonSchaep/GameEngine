#pragma once
#include "AIController.h"
#include "ObservingPointer.h"



class BeanAIController final : public AIController
{
public:
	BeanAIController(engine::GameObject* pGameObject) : AIController(pGameObject) {};

	void SetTargets(std::vector<engine::GameObject*> targets);
	void AddTarget(engine::GameObject* pTarget);

	virtual void Initialize() override;

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

	void FindClosestTarget();

	std::vector<engine::ObservingPointer<engine::GameObject>> m_Targets{};
	engine::ObservingPointer<engine::GameObject> m_CurrentTarget{};

	int m_LastTile{};
	int m_TargetTile{};

	bool m_IsEnemy{};

	engine::ObservingPointer<Level> m_pLevel;
};

