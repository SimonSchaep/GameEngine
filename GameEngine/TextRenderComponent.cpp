#include <stdexcept>
#include "TextRenderComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "ResourceManager.h"

namespace engine
{

	TextRenderComponent::TextRenderComponent(GameObject* pGameObject)
		: m_NeedsUpdate(true), m_Text(""), m_Color{ 255,255,255,255 }, TextureRenderComponent(pGameObject)
	{
	}

	TextRenderComponent::~TextRenderComponent() = default;

	void TextRenderComponent::Update()
	{
		if (!m_Font)
		{
			throw std::runtime_error(std::string("No font assigned: ") + SDL_GetError());
			return;
		}

		if (m_NeedsUpdate)
		{
			if (m_Text == "")
			{
				ClearTexture();
				return;
			}

			const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_FreeSurface(surf);
			SetTexture(std::make_unique<Texture2D>(texture));
			m_NeedsUpdate = false;
		}
	}

	void TextRenderComponent::Render()const
	{
		if (GetTexture() != nullptr)
		{
			auto pos = GetGameObject()->GetTransform()->GetWorldPosition();
			if (m_Alignment == TextAlignment::center)
			{
				pos.x -= GetTexture()->GetSize().x / 2;
			}
			else if (m_Alignment == TextAlignment::right)
			{
				pos.x -= GetTexture()->GetSize().x;
			}
			Renderer::GetInstance().RenderTexture(*GetTexture(), pos.x, pos.y);
		}
	}

	// This implementation uses the "dirty flag" pattern
	void TextRenderComponent::SetText(const std::string& text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}

	void TextRenderComponent::SetFont(const std::string& fileName, int fontSize)
	{
		m_Font = ResourceManager::GetInstance().LoadFont(fileName, fontSize);
		m_NeedsUpdate = true;
	}

	void TextRenderComponent::SetColor(SDL_Color color)
	{
		m_Color = color;
		m_NeedsUpdate = true;
	}


}