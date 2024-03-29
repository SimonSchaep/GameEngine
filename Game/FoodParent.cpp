#include "FoodParent.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "TimeManager.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "SoundSystem.h"

using namespace engine;

FoodParent::FoodParent(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_FallEvent = std::make_unique<Event<GameObject*, EventType>>();
	m_ReachedPlateEvent = std::make_unique<Event<FoodParent*>>();
}

void FoodParent::Initialize()
{
	GetGameObject()->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);

	for (auto pChild : GetGameObject()->GetChildren())
	{
		m_FoodElements.emplace_back(pChild);
		m_FoodElementStates.emplace_back(false);
		pChild->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);
	}

	m_DropSound = ServiceLocator::GetSoundSystem().AddClip("data/sounds/ingredientwalk.wav");
	m_BounceSound = ServiceLocator::GetSoundSystem().AddClip("data/sounds/bounce.wav");
}

void FoodParent::Update()
{
	if (m_IsFalling)
	{
		m_FallVelocity += m_FallAcceleration * TimeManager::GetInstance().GetDeltaTime();
		GetGameObject()->GetTransform()->Translate(0, m_FallVelocity * TimeManager::GetInstance().GetDeltaTime());
	}	
}

void FoodParent::HandleTriggerEnter(Collider* pOriginCollider, Collider* pHitCollider)
{

	if (pOriginCollider->GetGameObject() == GetGameObject()) //hit food parent
	{
		if (IsFalling() && m_FallVelocity < 0)
		{
			if (pHitCollider->GetGameObject()->HasTag("platform"))
			{
				StopFall(false);
			}
			else if (pHitCollider->GetGameObject()->HasTag("plate"))
			{
				ReachedPlate();
			}
			else if (pHitCollider->GetGameObject()->HasTag("foodparent"))
			{
				auto otherFoodParent = pHitCollider->GetGameObject()->GetComponent<FoodParent>();
				if (!otherFoodParent->HasReachedPlate())
				{
					if (otherFoodParent->GetGameObject()->GetTransform()->GetWorldPosition().y < GetGameObject()->GetTransform()->GetWorldPosition().y)
					{
						otherFoodParent->StartFall();
						m_FallVelocity = m_BounceVelocity;
						ServiceLocator::GetSoundSystem().Play(m_BounceSound);
					}					
				}
				else
				{
					ReachedPlate();
				}
			}
		}		
		return;
	}
	if (!pHitCollider->GetGameObject()->HasTag("Chef"))
	{
		return;
	}
	//hit food element
	//make corresponding food part fall
	auto it = std::find(m_FoodElements.begin(), m_FoodElements.end(), pOriginCollider->GetGameObject());
	if (it != m_FoodElements.end())
	{
		int id = int(it - m_FoodElements.begin());
		if (m_FoodElementStates[id]) //if element already dropped
		{
			return;
		}

		m_FoodElementStates[id] = true;

		ServiceLocator::GetSoundSystem().Play(m_DropSound);

		if (std::find(m_FoodElementStates.begin(), m_FoodElementStates.end(), false) == m_FoodElementStates.end()) //if all elements are dropped
		{
			StartFall();
		}
		else
		{
			DropFoodElement(id, false, false);
		}		
	}
}

void FoodParent::DropFoodElement(int elementId, bool skipDropLeftNeighbor, bool skipDropRightNeighbor)
{
	if (elementId == 0) //left
	{
		float rightNeighborPosY = m_FoodElements[elementId + 1]->GetTransform()->GetLocalPosition().y;

		auto newPos = m_FoodElements[elementId]->GetTransform()->GetLocalPosition();
		newPos.y = std::max(rightNeighborPosY + m_YPosForFoodDown, m_MinYPosForFoodDown);
		m_FoodElements[elementId]->GetTransform()->SetLocalPosition(newPos);
	}
	else if (elementId == int(m_FoodElements.size()) - 1) //right
	{
		float leftNeighborPosY = m_FoodElements[elementId - 1]->GetTransform()->GetLocalPosition().y;

		auto newPos = m_FoodElements[elementId]->GetTransform()->GetLocalPosition();
		newPos.y = std::max(leftNeighborPosY + m_YPosForFoodDown, m_MinYPosForFoodDown);
		m_FoodElements[elementId]->GetTransform()->SetLocalPosition(newPos);

	}
	else //middle
	{
		float leftNeighborPosY = m_FoodElements[elementId - 1]->GetTransform()->GetLocalPosition().y;
		float rightNeighborPosY = m_FoodElements[elementId + 1]->GetTransform()->GetLocalPosition().y;

		auto newPos = m_FoodElements[elementId]->GetTransform()->GetLocalPosition();
		newPos.y = std::max(std::max(leftNeighborPosY, rightNeighborPosY) + m_YPosForFoodDown, m_MinYPosForFoodDown);
		m_FoodElements[elementId]->GetTransform()->SetLocalPosition(newPos);

		//check if neighbors need to be lowered
		if (!skipDropLeftNeighbor && m_FoodElementStates[elementId - 1] && leftNeighborPosY >= newPos.y)
		{
			DropFoodElement(elementId - 1, false, true);
		}
		if (!skipDropRightNeighbor && m_FoodElementStates[elementId + 1] && rightNeighborPosY >= newPos.y)
		{
			DropFoodElement(elementId + 1, true, false);
		}
	}
}

void FoodParent::StopFall(bool isForced)
{
	if (m_FallVelocity == 0)return; //prevent falling multiple times

	if (!isForced && m_FallExtraLevels > 0)
	{
		m_FallExtraLevels--;
		m_FallVelocity = 0;
		return;
	}
	for (size_t i{}; i < m_FoodElementStates.size(); ++i)
	{
		m_FoodElementStates[i] = false;
	}
	m_FallVelocity = 0;
	
	m_FallEvent->NotifyObservers(GetGameObject(), EventType::foodStopFall);
	m_IsFalling = false;

	m_FallExtraLevels = m_EnemiesOnTop;
}

void FoodParent::ReachedPlate()
{
	if (m_ReachedPlate)return;

	StopFall(true);
	m_ReachedPlate = true;
	m_ReachedPlateEvent->NotifyObservers(this);
}

void FoodParent::StartFall()
{
	m_FallVelocity = 0;

	for (auto& foodElement : m_FoodElements)
	{
		auto newPos = foodElement->GetTransform()->GetLocalPosition();
		newPos.y = 0;
		foodElement->GetTransform()->SetLocalPosition(newPos);
	}

	for (size_t i{}; i < m_FoodElementStates.size(); ++i)
	{
		m_FoodElementStates[i] = true;
	}

	m_FallEvent->NotifyObservers(GetGameObject(), EventType::foodStartFall);
	m_IsFalling = true;
}

void FoodParent::EnemyEntered()
{
	m_FallExtraLevels++;
	m_EnemiesOnTop++;
}

void FoodParent::EnemyLeft()
{
	m_FallExtraLevels--;
	m_EnemiesOnTop--;
}
