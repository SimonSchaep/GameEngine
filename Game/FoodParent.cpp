#include "FoodParent.h"
#include "GameObject.h"
#include "FoodElement.h"
#include "BoxCollider.h"
#include <iostream>

void FoodParent::Initialize()
{
	for (auto pChild : GetGameObject()->GetChildren())
	{
		m_FoodElements.push_back(pChild);
		m_FoodElementStates.push_back(false);
		pChild->GetComponent<BoxCollider>()->GetOnTriggerEnterEvent()->AddObserver(this);
	}
}

void FoodParent::Notify(Collider* pOriginCollider, Collider* pHitCollider)
{
	//make corresponding food part fall
	if (!pHitCollider->GetGameObject()->HasTag("Chef"))
	{
		return;
	}
	auto it = std::find(m_FoodElements.begin(), m_FoodElements.end(), pOriginCollider->GetGameObject());
	if (it != m_FoodElements.end())
	{
		int id = int(it - m_FoodElements.begin());
		if (m_FoodElementStates[id]) //if element already dropped
		{
			return;
		}

		m_FoodElementStates[id] = true;

		if (std::find(m_FoodElementStates.begin(), m_FoodElementStates.end(), false) == m_FoodElementStates.end()) //if all elements are dropped
		{
			Fall();
		}

		glm::vec3 newPos = pOriginCollider->GetGameObject()->GetTransform()->GetLocalPosition();
		newPos.y = m_YPosForFoodDown;
		pOriginCollider->GetGameObject()->GetTransform()->SetLocalPosition(newPos);
	}
}

void FoodParent::Fall()
{
	for (auto& foodElement : m_FoodElements)
	{
		auto newPos = foodElement->GetTransform()->GetLocalPosition();
		newPos.y = 0;
		foodElement->GetTransform()->SetLocalPosition(newPos);
	}
	for (size_t i{}; i < m_FoodElementStates.size(); ++i)
	{
		m_FoodElementStates[i] = false;
	}
	//fallcomponent.fall
}
