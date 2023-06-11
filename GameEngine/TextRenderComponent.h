#pragma once
#include "TextureRenderComponent.h"
#include "Transform.h"
#include <SDL_ttf.h>
#include <string>
#include <memory>

namespace engine
{
	class Font;

	class TextRenderComponent : public TextureRenderComponent
	{
	public:
		enum class TextAlignment
		{
			left,
			right,
			center,
		};

		void Update() override;
		virtual void Render()const override;

		void SetText(const std::string& text);
		void SetFont(const std::string& fileName, int fontSize);
		void SetColor(SDL_Color color);
		void SetTextAlignment(TextAlignment alignment) { m_Alignment = alignment; }

		TextRenderComponent(GameObject* pGameObject);
		virtual ~TextRenderComponent();
	private:
		bool m_NeedsUpdate{};
		std::string m_Text{};
		std::unique_ptr<Font> m_Font{};
		SDL_Color m_Color{};
		TextAlignment m_Alignment{};
	};
}