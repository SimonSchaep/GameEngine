#pragma once
#include "RenderComponent.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

//todo: add color modifier, for example to modify transparency

namespace engine
{

	class Texture2D;

	class TextureRenderComponent : public RenderComponent
	{
	public:
		TextureRenderComponent(GameObject* pGameObject);
		virtual ~TextureRenderComponent();

		void SetTexture(const std::string& filename);
		Texture2D* GetTexture()const { return m_Texture.get(); }
		void ClearTexture();

		void SetSize(const glm::vec2& size) { m_Size = size; }
		glm::vec2 GetSize();

		virtual void Render()const override;

	protected:
		void SetTexture(std::unique_ptr<Texture2D> texture);

	private:
		std::unique_ptr<Texture2D> m_Texture{};
		glm::vec2 m_Size{};
	};

}