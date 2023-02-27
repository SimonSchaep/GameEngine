#pragma once
#include <memory>
#include <string>
#include "BaseComponent.h"

class Texture2D;

class RenderComponent : public BaseComponent
{
public:
	RenderComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~RenderComponent() = default;

	void SetTexture(const std::string& filename);

	virtual void Initialize() override;

	virtual void Update() override;

	virtual void Render()const;

protected:
	Texture2D* GetTexture()const { return m_texture.get(); };
	void SetTexture(std::shared_ptr<Texture2D> texture) { m_texture = std::move(texture); };

private:
	std::shared_ptr<Texture2D> m_texture{};
};

