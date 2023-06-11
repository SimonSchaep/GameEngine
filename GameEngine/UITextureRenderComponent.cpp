#include "UITextureRenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

namespace engine
{

	UITextureRenderComponent::UITextureRenderComponent(GameObject* pGameObject)
		:UIRenderComponent(pGameObject)
	{
	}

	UITextureRenderComponent::~UITextureRenderComponent() = default;

	void UITextureRenderComponent::SetTexture(const std::string& filename)
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	}

	void UITextureRenderComponent::ClearTexture()
	{
		m_Texture.reset();
	}

	glm::vec2 UITextureRenderComponent::GetSize()
	{
		if (m_Size != glm::vec2{ 0, 0 }) return m_Size;
		else return m_Texture->GetSize();
	}

	void UITextureRenderComponent::RenderUI()
	{
		if (!m_Texture)
		{
			return;
		}
		const auto& pos = GetGameObject()->GetTransform()->GetWorldPosition();
		if (m_Size.x != 0 && m_Size.y != 0)
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, m_Size.x, m_Size.y);
		}
		else
		{
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
		}
	}

	void UITextureRenderComponent::SetTexture(std::unique_ptr<Texture2D> texture)
	{
		m_Texture = std::move(texture);
	}
}