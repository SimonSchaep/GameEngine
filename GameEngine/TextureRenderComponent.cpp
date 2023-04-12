#include "TextureRenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

TextureRenderComponent::TextureRenderComponent(GameObject* pGameObject)
	:RenderComponent(pGameObject)
{
}

TextureRenderComponent::~TextureRenderComponent() = default;

void TextureRenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void TextureRenderComponent::ClearTexture()
{
	m_Texture.reset();
}

void TextureRenderComponent::Render()const
{
	if (!m_Texture)
	{
		return;
	}
	const auto& pos = GetGameObject()->GetTransform()->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

Texture2D* TextureRenderComponent::GetTexture() const
{
	return m_Texture.get();
}

void TextureRenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
{
	m_Texture = std::move(texture);
}
