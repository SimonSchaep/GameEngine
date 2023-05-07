#pragma once
#include <memory>
#include <string>
#include "RenderComponent.h"

namespace engine
{

	class Texture2D;

	class TextureRenderComponent : public RenderComponent
	{
	public:
		TextureRenderComponent(GameObject* pGameObject);
		virtual ~TextureRenderComponent();

		void SetTexture(const std::string& filename);
		Texture2D* GetTexture()const { return m_Texture.get(); };
		void ClearTexture();

		virtual void Render()const override;

	protected:
		void SetTexture(std::unique_ptr<Texture2D> texture);

	private:
		std::unique_ptr<Texture2D> m_Texture{};
	};

}