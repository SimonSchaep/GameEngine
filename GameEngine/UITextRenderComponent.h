#pragma once
#include <string>
#include <memory>
#include "UITextureRenderComponent.h"
#include "Transform.h"
#include <SDL_ttf.h>

namespace engine
{

	class Font;

	class UITextRenderComponent : public UITextureRenderComponent
	{
	public:
		enum class TextAlignment
		{
			left,
			right,
			center,
		};

		void Update() override;
		virtual void RenderUI() override;

		void SetText(const std::string& text);
		void SetFont(const std::string& fileName, int fontSize);
		void SetColor(SDL_Color color);
		void SetTextAlignment(TextAlignment alignment) { m_Alignment = alignment; }

		UITextRenderComponent(GameObject* pGameObject);
		virtual ~UITextRenderComponent();
	private:
		bool m_NeedsUpdate{};
		std::string m_Text{};
		std::unique_ptr<Font> m_Font{};
		SDL_Color m_Color{};
		TextAlignment m_Alignment{};
	};
}