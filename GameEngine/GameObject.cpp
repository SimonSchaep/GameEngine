#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "RenderComponent.h"

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

const glm::vec3& GameObject::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		if (m_Parent)
		{
			m_WorldTransform.SetPosition(m_Parent->GetWorldPosition() + m_LocalTransform.GetPosition());
		}
		else
		{
			m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
		}
	}
	return m_WorldTransform.GetPosition();
}

void GameObject::SetLocalPosition(float x, float y)
{
	m_LocalTransform.SetPosition(x, y, 0.0f);
	m_IsPositionDirty = true;
}

void GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalTransform.SetPosition(pos);
	m_IsPositionDirty = true;
}

void GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
{
	if (!m_Parent)
	{
		m_LocalTransform.SetPosition(GetWorldPosition());
	}
	else
	{
		m_IsPositionDirty = true;
		if (keepWorldPosition)
		{
			m_LocalTransform.SetPosition(GetLocalPosition() - pParent->GetWorldPosition());
		}
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
