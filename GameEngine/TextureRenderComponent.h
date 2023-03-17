#pragma once
#include <memory>
#include <string>
#include "RenderComponent.h"

class Texture2D;

class TextureRenderComponent : public RenderComponent
{
public:
	TextureRenderComponent(GameObject* pGameObject) :RenderComponent(pGameObject) {};
	virtual ~TextureRenderComponent() = default;

	void SetTexture(const std::string& filename);

	virtual void Initialize() override;

	virtual void Update() override;

	virtual void Render() override;

protected:
	Texture2D* GetTexture()const { return m_texture.get(); };
	void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = std::move(texture); };

private:
	std::shared_ptr<Texture2D> m_texture{};
};

