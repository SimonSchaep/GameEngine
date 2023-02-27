#pragma once
#include <string>
#include <memory>
#include "RenderComponent.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextRenderComponent : public RenderComponent
	{
	public:
		void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);

		TextRenderComponent(GameObject* pGameObject, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextRenderComponent() = default;
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;
	private:
		bool m_needsUpdate{};
		std::string m_text{};
		std::shared_ptr<Font> m_font{};
	};
}
