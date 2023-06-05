#include "FoodParent.h"
#include "GameObject.h"
#include "FoodElement.h"
#include "BoxCollider.h"
#include "TimeManager.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "SoundSystem.h"

using namespace engine;

FoodParent::FoodParent(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_FallEvent = std::make_unique<Event<GameObject*, bool>>();
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

	m_DropSound = ServiceLocator::GetSoundSystem().AddClip("data/foodelementdrop.wav");
}

void FoodParent::Update()
{
	if (m_IsFalling)
	{
		m_FallVelocity += m_FallAcceleration * TimeManager::GetInstance().GetDeltaTime();
		GetGameObject()->GetTransform()->Translate(0, m_FallVelocity * TimeManager::GetInstance().GetDeltaTime(), 0);
	}	
}

void FoodParent::Notify(Collider::TriggerType triggerType, Collider* pOriginCollider, Collider* pHitCollider)
{
	switch (triggerType)
	{
	case engine::Collider::TriggerType::enter:
		HandleTriggerEnter(pOriginCollider, pHitCollider);
		break;
	case engine::Collider::TriggerType::exit:
		HandleTriggerExit(pOriginCollider, pHitCollider);
		break;
	case engine::Collider::TriggerType::stay:
		HandleTriggerStay(pOriginCollider, pHitCollider);
		break;
	default:
		break;
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
				StopFall();
			}
			else if (pHitCollider->GetGameObject()->HasTag("plate"))
			{
				m_ReachedPlate = true;
				StopFall();
			}
			else if (pHitCollider->GetGameObject()->HasTag("foodparent"))
			{
				auto otherFoodParent = pHitCollider->GetGameObject()->GetComponent<FoodParent>();
				if (!otherFoodParent->ReachedPlate())
				{
					if (!otherFoodParent->IsFalling() && otherFoodParent->GetGameObject()->GetTransform()->GetWorldPosition().y < GetGameObject()->GetTransform()->GetWorldPosition().y)
					{
						otherFoodParent->StartFall();
						m_FallVelocity = m_BounceVelocity;
					}					
				}
				else
				{
					m_ReachedPlate = true;
					StopFall();
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

		//ServiceLocator::GetLogger().LogLine("drop food element");
		ServiceLocator::GetSoundSystem().Play(m_DropSound);

		if (std::find(m_FoodElementStates.begin(), m_FoodElementStates.end(), false) == m_FoodElementStates.end()) //if all elements are dropped
		{
			StartFall();
		}
		else
		{
			//glm::vec3 newPos = pOriginCollider->GetGameObject()->GetTransform()->GetLocalPosition();
			//newPos.y = m_YPosForFoodDown;
			//pOriginCollider->GetGameObject()->GetTransform()->SetLocalPosition(newPos);

			DropFoodElement(id, false, false);
		}		
	}
}

void FoodParent::HandleTriggerExit(Collider* /*pOriginCollider*/, Collider* /*pHitCollider*/)
{
}

void FoodParent::HandleTriggerStay(Collider* /*pOriginCollider*/, Collider* /*pHitCollider*/)
{
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

void FoodParent::StopFall()
{
	for (size_t i{}; i < m_FoodElementStates.size(); ++i)
	{
		m_FoodElementStates[i] = false;
	}
	m_FallVelocity = 0;
	m_FallEvent->NotifyObservers(GetGameObject(), false);
	m_IsFalling = false;
}

void FoodParent::StartFall()
{
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

	m_FallEvent->NotifyObservers(GetGameObject(), true);
	m_IsFalling = true;
}
