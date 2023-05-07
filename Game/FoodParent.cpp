#include "FoodParent.h"
#include "GameObject.h"
#include "FoodElement.h"
#include "BoxCollider.h"

void FoodParent::Initialize()
{
	for (auto pChild : GetGameObject()->GetChildren())
	{
		bool isFoodElement = pChild->GetComponent<FoodElement>();
		if (isFoodElement)
		{
			m_FoodElements.push_back(pChild);
			m_FoodElementStates.push_back(false);
			pChild->GetComponent<BoxCollider>()->GetOnCollisionEnterEvent()->AddObserver(this);
		}
	}
}

void FoodParent::Notify(Collider* pOriginCollider, Collider* /*pHitCollider*/)
{
	auto it = std::find(m_FoodElements.begin(), m_FoodElements.end(), pOriginCollider->GetGameObject());
	if (it != m_FoodElements.end())
	{
		int id = int(it - m_FoodElements.begin());
		auto newPos = pOriginCollider->GetGameObject()->GetTransform()->GetWorldPosition();
		newPos.y = GetLowestYPosOfNeighbors(id) - m_YLowerPerFoodDown;
		pOriginCollider->GetGameObject()->GetTransform()->SetWorldPosition(newPos);
		m_FoodElementStates[id] = true;
	}
}

float FoodParent::GetLowestYPosOfNeighbors(int foodElementIndex)
{
	if (foodElementIndex == 0) //if first
	{
		return m_FoodElements[foodElementIndex + 1]->GetTransform()->GetWorldPosition().y;
	}
	else if (foodElementIndex == m_FoodElements.size() - 1) //if last
	{
		return m_FoodElements[foodElementIndex - 1]->GetTransform()->GetWorldPosition().y;
	}
	else
	{
		return std::min(m_FoodElements[foodElementIndex + 1]->GetTransform()->GetWorldPosition().y,
			m_FoodElements[foodElementIndex - 1]->GetTransform()->GetWorldPosition().y);
	}
}
