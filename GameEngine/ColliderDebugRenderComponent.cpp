#include "ColliderDebugRenderComponent.h"
#include "Collider.h"
#include "GameObject.h"

engine::ColliderDebugRenderComponent::ColliderDebugRenderComponent(GameObject* pGameObject) :RenderComponent(pGameObject)
{
}

engine::ColliderDebugRenderComponent::~ColliderDebugRenderComponent()
{
}

void engine::ColliderDebugRenderComponent::Initialize()
{
	m_Colliders = GetGameObject()->GetAllComponentsOfType<Collider>();
}

void engine::ColliderDebugRenderComponent::Render() const
{
	//draw colliders
}
