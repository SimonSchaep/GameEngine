#include "EnemyAIController.h"
#include "GameObject.h"
#include "Utility.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "Scene.h"
#include "GameManager.h"
#include "Level.h"
#include "EnemyLogic.h"
#include "TimeManager.h"
#include <deque>

using namespace engine;

EnemyAIController::EnemyAIController(engine::GameObject* pGameObject, float spawnDelay)
	: AIController(pGameObject), m_SpawnDelay{ spawnDelay }, m_SpawnDelayTimer{ spawnDelay }
{}

void EnemyAIController::Initialize()
{
	AIController::Initialize();

	m_pLevel = GetControlledGameObject()->GetScene()->FindGameObjectByName("Level")->GetComponent<Level>();
	if (!m_pLevel)
	{
		ServiceLocator::GetLogger().LogLine("No level component on an object named \"Level\" found", LogType::error);
		return;
	}

	m_TargetTile = m_pLevel->GetIndexOfPos(GetControlledGameObject()->GetTransform()->GetWorldPosition());

	auto pGameManager = GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>();
	pGameManager->GetOnRespawnCharacters()->AddObserver(this);

	UpdateTargets();
}

void EnemyAIController::Notify(EventType type)
{
	AIController::Notify(type);

	if (type == EventType::respawnCharacters)
	{
		m_TargetTileIsInvalid = true;
		UpdateTargets();
		m_SpawnDelayTimer = m_SpawnDelay;
	}
}

void EnemyAIController::SetControlledObject(engine::GameObject* pControlledObject)
{
	AIController::SetControlledObject(pControlledObject);

	if (m_pLevel)
	{
		m_TargetTile = m_pLevel->GetIndexOfPos(GetControlledGameObject()->GetTransform()->GetWorldPosition());
	}

	m_EnemyLogic = pControlledObject->GetComponent<EnemyLogic>();
}

void EnemyAIController::ProcessAIDecisions()
{
	if (m_EnemyLogic->IsDead() || m_EnemyLogic->IsFalling())
	{
		m_TargetTileIsInvalid = true;
		return;
	}
	if (IsPaused() || m_EnemyLogic->IsStunned()) return;

	if (m_SpawnDelayTimer > 0)
	{
		m_SpawnDelayTimer -= TimeManager::GetInstance().GetDeltaTime();
		if (m_SpawnDelayTimer > 0)
		{
			return;
		}
	}

	if (m_TargetTileIsInvalid)
	{
		m_TargetTile = m_pLevel->GetIndexOfPos(GetControlledGameObject()->GetTransform()->GetWorldPosition());
		m_LastTile = m_TargetTile;
		m_TargetTileIsInvalid = false;
	}

	auto& ownPos = GetControlledGameObject()->GetTransform()->GetWorldPosition();
	auto targetTilePos = m_pLevel->GetCenterOfCell(m_TargetTile);

	if (HasReachedTargetTile())
	{
		FindNewTargetTile();
	}

	const float margin{1};

	if (ownPos.x > targetTilePos.x + margin)
	{
		MoveLeft();
	}
	else if (ownPos.x < targetTilePos.x - margin)
	{
		MoveRight();
	}
	if (ownPos.y > targetTilePos.y + margin)
	{
		MoveDown();
	}
	else if (ownPos.y < targetTilePos.y - margin)
	{
		MoveUp();
	}
}

bool EnemyAIController::HasReachedTargetTile()
{
	auto& currentPos = GetControlledGameObject()->GetTransform()->GetWorldPosition();
	return m_pLevel->GetIndexOfPos(currentPos) == m_TargetTile
		&& m_pLevel->IsInCenterOfElementX(currentPos, 2) && m_pLevel->IsInCenterOfElementY(currentPos, 2);
}

void EnemyAIController::FindNewTargetTile() //target tile will be current tile in here, since it gets called after we reach target tile
{
	FindClosestTarget();
	if (!m_CurrentTarget)
	{
		ServiceLocator::GetLogger().LogLine("No target assigned to enemyaicontroller", LogType::debug);
		return;
	}

	auto adjacentTiles = m_pLevel->GetAdjacentNavigableTiles(m_TargetTile, false);

	int newTargetTile{-1};

	if (adjacentTiles.size() == 0)
	{
		//include cheats so there is a path
		adjacentTiles = m_pLevel->GetAdjacentNavigableTiles(m_TargetTile, true);
	}

	if (adjacentTiles.size() > 2) //if more than 2 options
	{
		//check if should give ladder priority
		auto ownPos = m_pLevel->GetCenterOfCell(m_TargetTile); //make sure we take the centered pos
		auto& targetPos = m_CurrentTarget->GetTransform()->GetWorldPosition();

		for (auto tile : adjacentTiles)
		{
			if (tile == m_LastTile)continue;

			constexpr float epsilon{ 0.001f };
			if ((tile - m_TargetTile > 1 && targetPos.y > ownPos.y + epsilon) //if row above and chef above or equal
				|| (tile - m_TargetTile < -1 && targetPos.y < ownPos.y - epsilon)) //or row below and chef below or equal
			{
				//take ladder
				newTargetTile = tile;
			}
		}

		if (newTargetTile == -1) //if no ladder priority given
		{
			//do A*
			newTargetTile = FindTargetTileAStar();
		}
	}
	else if(adjacentTiles.size() >= 1)
	{
		if (adjacentTiles.size() == 2) //2 options
		{
			//take the one that we don't come from
			if (adjacentTiles[0] != m_LastTile)
			{
				newTargetTile = adjacentTiles[0];
			}
			else
			{
				newTargetTile = adjacentTiles[1];
			}
		}
		else //only one option, so dead end, so can go back
		{
			newTargetTile = adjacentTiles[0];
		}
	}
	else //no adjacent tiles, should not be possible
	{
		ServiceLocator::GetLogger().LogLine("No adjacent tiles, this shouldn't be possible", LogType::warning);
	}

	m_LastTile = m_TargetTile;
	m_TargetTile = newTargetTile;
}

//Todo: Can probably still be optimized, but it works for now
int EnemyAIController::FindTargetTileAStar()
{
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
		auto adjacentTiles = m_pLevel->GetAdjacentNavigableTiles(currentTile->tile, true);
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
		return m_TargetTile;
	}
	else
	{
		//Trace back
		while (currentTile && currentTile->pPrevTile && currentTile->pPrevTile->tile != m_TargetTile)
		{
			currentTile = currentTile->pPrevTile;
		}
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

void EnemyAIController::UpdateTargets()
{
	m_Targets.clear();
	for (auto pTarget : GetScene()->FindAllGameObjectsWithTag("Chef"))
	{
		m_Targets.emplace_back(pTarget);
	}
}

void EnemyAIController::FindClosestTarget()
{
	float closestDistanceSq{FLT_MAX};
	for (auto pTarget : m_Targets)
	{
		auto distanceSq = utility::DistanceSquared(pTarget->GetTransform()->GetWorldPosition(), GetControlledGameObject()->GetTransform()->GetWorldPosition());
		if (distanceSq < closestDistanceSq)
		{
			closestDistanceSq = distanceSq;
			m_CurrentTarget = pTarget;
		}
	}
}
