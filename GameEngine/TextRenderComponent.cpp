#include <stdexcept>
#include <SDL_ttf.h>
#include "TextRenderComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

dae::TextRenderComponent::TextRenderComponent(GameObject* pGameObject, const std::string& text, std::shared_ptr<Font> font) 
	: m_needsUpdate(true), m_text(text), m_font(std::move(font)), RenderComponent(pGameObject)
{ }

void dae::TextRenderComponent::Update()
{
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
		m_texture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextRenderComponent::Render() const
{
	if (m_texture != nullptr)
	{
		const auto& pos = m_pGameObject->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextRenderComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

