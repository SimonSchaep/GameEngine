#include "RenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

void RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void RenderComponent::Initialize()
{
}

void RenderComponent::Update()
{
}

void RenderComponent::Render() const
{
	const auto& pos = GetGameObject()->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}