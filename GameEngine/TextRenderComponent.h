#pragma once
#include <string>
#include <memory>
#include "TextureRenderComponent.h"
#include "Transform.h"

class Font;
class Texture2D;
class TextRenderComponent : public TextureRenderComponent
{
public:
	void Update() override;
	virtual void Render() const override;

	void SetText(const std::string& text);
	void SetFont(std::shared_ptr<Font> font);

	TextRenderComponent(GameObject* pGameObject);
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
