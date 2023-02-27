#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Initialize()
{
	for (auto& c : m_Components)
	{
		c->Initialize();
	}
}

void dae::GameObject::Update()
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

void dae::GameObject::Render() const
{
	for (auto& renderer : GetAllComponentsOfType<RenderComponent>())
	{
		renderer->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}
