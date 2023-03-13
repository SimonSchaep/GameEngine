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
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->Update();
		if (m_Components[i]->IsMarkedForDeletion())
		{
			m_ToDeleteIndexes.push_back(i);
		}
	}

	//Delete Components
	for (size_t i : m_ToDeleteIndexes)
	{
		m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), m_Components[i]));
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

void GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (!m_Parent)
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
