#include <stdexcept>
#include <SDL_ttf.h>
#include "TextRenderComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

TextRenderComponent::TextRenderComponent(GameObject* pGameObject) 
	: m_needsUpdate(true), m_text(""), TextureRenderComponent(pGameObject)
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
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
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

void TextRenderComponent::Render()
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
}


