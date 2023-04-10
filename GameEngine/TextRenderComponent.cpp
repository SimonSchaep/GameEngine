#include <stdexcept>
#include "TextRenderComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

TextRenderComponent::TextRenderComponent(GameObject* pGameObject) 
	: m_needsUpdate(true), m_text(""), m_Color{255,255,255,255}, TextureRenderComponent(pGameObject)
{ }

void TextRenderComponent::Update()
{
	if (!m_font)
	{
		throw std::runtime_error(std::string("No font assigned: ") + SDL_GetError());
		return;
	}

	if (m_needsUpdate)
	{
		if (m_text == "")
		{
			ClearTexture();
			return;
		}

		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_Color);
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
		SetTexture(std::make_shared<Texture2D>(texture));
		m_needsUpdate = false;
	}
}

void TextRenderComponent::Render()const
{
	if (GetTexture() != nullptr)
	{
		const auto& pos = GetGameObject()->GetTransform()->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*GetTexture(), pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void TextRenderComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void TextRenderComponent::SetFont(std::shared_ptr<Font> font)
{
	m_font = std::move(font);
	m_needsUpdate = true;
}

void TextRenderComponent::SetColor(SDL_Color color)
{
	m_Color = color;
	m_needsUpdate = true;
}


