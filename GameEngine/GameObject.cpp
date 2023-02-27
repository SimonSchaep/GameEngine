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
	std::vector<size_t> m_ToDeleteIndexes{};
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->Update();
		if (m_Components[i]->IsMarkedForDeletion())
		{
			m_ToDeleteIndexes.push_back(i);
		}
	}
	for (int i{ int(m_ToDeleteIndexes.size() - 1) }; i >= 0; --i)
	{
		m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), m_Components[m_ToDeleteIndexes[i]]));
	}
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
	m_transform.SetPosition(x, y, 0.0f);
}
