#pragma once
#include <string>
#include <memory>
#include "TextureRenderComponent.h"
#include "Transform.h"
#include <SDL_ttf.h>

namespace engine
{

	class Font;

	class TextRenderComponent : public TextureRenderComponent
	{
	public:
		void Update() override;
		virtual void Render()const override;

		void SetText(const std::string& text);
		void SetFont(const std::string& fileName, int fontSize);
		void SetColor(SDL_Color color);

		TextRenderComponent(GameObject* pGameObject);
		virtual ~TextRenderComponent();
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;
	private:
		bool m_NeedsUpdate{};
		std::string m_Text{};
		std::unique_ptr<Font> m_Font{};
		SDL_Color m_Color{};
	};
}