#include "BeanAIController.h"
#include "GameObject.h"
#include "Utility.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "Scene.h"
#include "Level.h"
#include <deque>

using namespace engine;

void BeanAIController::SetTargets(std::vector<engine::GameObject*> targets)
{
	m_Targets.clear();
	for (auto pTarget : targets)
	{
		AddTarget(pTarget);
	}
}

void BeanAIController::AddTarget(engine::GameObject* pTarget)
{
	m_Targets.emplace_back(pTarget);
}

void BeanAIController::Initialize()
{
	m_pLevel = GetControlledGameObject()->GetScene()->FindGameObjectByName("Level")->GetComponent<Level>();
	if (!m_pLevel)
	{
		ServiceLocator::GetLogger().LogLine("No level component on an object named \"Level\" found", LogType::error);
		return;
	}

	m_TargetTile = m_pLevel->GetIndexOfPos(GetControlledGameObject()->GetTransform()->GetWorldPosition());
}

void BeanAIController::SetControlledObject(engine::GameObject* pControlledObject)
{
	AIController::SetControlledObject(pControlledObject);

	m_IsEnemy = GetControlledGameObject()->HasTag("Enemy");

	if (m_pLevel)
	{
		m_TargetTile = m_pLevel->GetIndexOfPos(GetControlledGameObject()->GetTransform()->GetWorldPosition());
	}
}

void BeanAIController::ProcessAIDecisions()
{
	auto& ownPos = GetControlledGameObject()->GetTransform()->GetWorldPosition();
	auto targetPos = m_pLevel->GetCenterOfCell(m_TargetTile);

	if (HasReachedTargetTile())
	{
		FindNewTargetTile();
	}

	const float margin{1};

	if (ownPos.x > targetPos.x + margin)
	{
		MoveLeft();
	}
	else if (ownPos.x < targetPos.x - margin)
	{
		MoveRight();
	}
	if (ownPos.y > targetPos.y + margin)
	{
		MoveDown();
	}
	else if (ownPos.y < targetPos.y - margin)
	{
		MoveUp();
	}

	////take ladder if possible
	//if (m_CurrentDirection.y != -1 && CanMoveUp() && targetPos.y >= ownPos.y)
	//{
	//	MoveUp();
	//	m_CurrentDirection = { 0,1 };
	//	return;
	//}
	//if (m_CurrentDirection.y != 1 && CanMoveDown() && targetPos.y <= ownPos.y)
	//{
	//	MoveDown();
	//	m_CurrentDirection = { 0,-1 };
	//	return;
	//}

	//if (m_CurrentDirection.x != -1 && CanMoveRight() && targetPos.x >= ownPos.x)
	//{
	//	MoveRight();
	//	m_CurrentDirection = { 1,0 };
	//	return;
	//}
	//if (m_CurrentDirection.x != 1 && CanMoveLeft() && targetPos.x <= ownPos.x)
	//{
	//	MoveLeft();
	//	m_CurrentDirection = { -1,0 };
	//	return;
	//}

	//Move(m_CurrentDirection);
}

bool BeanAIController::HasReachedTargetTile()
{
	auto& currentPos = GetControlledGameObject()->GetTransform()->GetWorldPosition();
	return m_pLevel->GetIndexOfPos(currentPos) == m_TargetTile
		&& m_pLevel->IsInCenterOfElementX(currentPos, 1) && m_pLevel->IsInCenterOfElementY(currentPos, 1);
}

void BeanAIController::FindNewTargetTile()
{
	FindClosestTarget();
	if (!m_CurrentTarget)
	{
		ServiceLocator::GetLogger().LogLine("No target assigned to beanaicontroller", LogType::debug);
		return;
	}

	int newTargetTile = FindTargetTileAStar();

	m_LastTile = m_TargetTile;
	m_TargetTile = newTargetTile;
}

//Todo: Can probably still be optimized, but it works for now
int BeanAIController::FindTargetTileAStar()
{
	ServiceLocator::GetLogger().LogLine("Find new path", LogType::debug);

	std::deque<std::unique_ptr<AStarTile>> openList{}; //deque so we can add adjacent tiles to front
	std::vector<std::unique_ptr<AStarTile>> closedList{};

	int currentTargetTileIndex = m_pLevel->GetIndexOfPos(m_CurrentTarget->GetTransform()->GetWorldPosition());

	//add starting tile to open list
	AStarTile* currentTile{};
	openList.emplace_back(std::make_unique<AStarTile>(m_TargetTile, nullptr, 0));

	while (!openList.empty())
	{
		//move tile from open list to closed list
		currentTile = openList.back().get();
		closedList.emplace_back(std::move(openList.back()));
		openList.pop_back();
		if (currentTile->tile == currentTargetTileIndex)
		{
			break;
		}

		//for each adjacent tile
		auto adjacentTiles = m_pLevel->GetAdjacentNavigableTiles(currentTile->tile, m_IsEnemy);
		for (auto tile : adjacentTiles)
		{
			if (tile == m_LastTile)
			{
				continue;
			}

			auto itOpen = std::find_if(openList.begin(), openList.end(), [tile](const std::unique_ptr<AStarTile>& aStarTile)
				{
					return tile == aStarTile->tile;
				});
			auto itClosed = std::find_if(closedList.begin(), closedList.end(), [tile](const std::unique_ptr<AStarTile>& aStarTile)
				{
					return tile == aStarTile->tile;
				});
			//if it's on the closed list
			if (itClosed != closedList.end())
			{
				//if the new path is shorter
				if (currentTile->cost + 1 < itClosed->get()->cost)
				{
					//set prev tile to this prev tile
					itClosed->get()->pPrevTile = currentTile;
					itClosed->get()->cost = currentTile->cost + 1;
				}
				continue;
			}
			//if it's not on the open list
			if (itOpen == openList.end())
			{
				//add it to the open list
				openList.emplace_front(std::make_unique<AStarTile>(tile, currentTile, currentTile->cost + 1));
			}
			//if the new path is shorter
			else if(currentTile->cost + 1 < itOpen->get()->cost)
			{
				//set prev tile to this prev tile
				itOpen->get()->pPrevTile = currentTile;
				itOpen->get()->cost = currentTile->cost + 1;
			}
		}
	}

	if (openList.empty())
	{
		ServiceLocator::GetLogger().LogLine("No path to current target for bean aicontroller found", LogType::debug);
		return m_TargetTile;
	}
	else
	{
		//Trace back
		while (currentTile && currentTile->pPrevTile && currentTile->pPrevTile->tile != m_TargetTile)
		{
			currentTile = currentTile->pPrevTile;
			ServiceLocator::GetLogger().Log(std::to_string(currentTile->tile) + ", ", LogType::debug);
		}
		ServiceLocator::GetLogger().Log("\n", LogType::debug);
		if (!currentTile)
		{
			return m_TargetTile;
		}
		else
		{
			return currentTile->tile;
		}
	}
}

void BeanAIController::FindClosestTarget()
{
	float closestDistanceSq{FLT_MAX};
	for (auto pTarget : m_Targets)
	{
		auto distanceSq = utility::DistanceSquared(pTarget->GetTransform()->GetWorldPosition(), GetGameObject()->GetTransform()->GetWorldPosition());
		if (distanceSq < closestDistanceSq)
		{
			closestDistanceSq = distanceSq;
			m_CurrentTarget = pTarget;
		}
	}
}
