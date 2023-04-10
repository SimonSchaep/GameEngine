#include "TextureRenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

void TextureRenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureRenderComponent::ClearTexture()
{
	m_texture.reset();
}

void TextureRenderComponent::Render()const
{
	if (!m_texture)
	{
		return;
	}
	const auto& pos = GetGameObject()->GetTransform()->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}