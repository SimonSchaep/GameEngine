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
	for (auto& c : m_Components)
	{
		c->Update();
	}
}

void dae::GameObject::Render() const
{
	for (auto& renderer : GetAllComponents<RenderComponent>())
	{
		renderer->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
