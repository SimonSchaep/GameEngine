#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "RenderComponent.h"
#include <iostream>

GameObject::GameObject()
	: m_Transform{std::make_unique<Transform>(this)}
{
}

GameObject::~GameObject() = default;

void GameObject::Initialize()
{
	for (auto& c : m_Components)
	{
		c->Initialize();
	}
}

void GameObject::Update()
{
	//Update Components
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->Update();
		if (m_Components[i]->IsMarkedForDeletion())
		{
			m_ToDeleteIndexes.push_back(i);
		}
	}

	//Delete Components - reverse order because it removes Components by index
	//If the Components at idx 0 and 1 need to be deleted and the one at 0 is deleted first, the Components at idx 1 will be at 0 when we delete it
	for (int i{ int(m_ToDeleteIndexes.size()) - 1 }; i >= 0; --i)
	{
		m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), m_Components[m_ToDeleteIndexes[i]]));
	}
	m_ToDeleteIndexes.clear();
}

void GameObject::Render() const
{
	for (auto& renderer : GetAllComponentsOfType<RenderComponent>())
	{
		renderer->Render();
	}
}

void GameObject::MarkForDeletion(bool includeChildren)
{
	m_IsMarkedForDeletion = true;
	if (includeChildren)
	{
		for (auto pChild : m_Children)
		{
			pChild->MarkForDeletion(includeChildren);
		}
	}
}

void GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (!pParent && keepWorldPosition)
	{
		m_Transform->SetLocalPosition(m_Transform->GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			m_Transform->SetLocalPosition(m_Transform->GetLocalPosition() - pParent->GetTransform()->GetWorldPosition());
		}
		m_Transform->SetDirty(true);
	}

	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = pParent;
	if (m_Parent)
	{
		m_Parent->AddChild(this);
	}
}
