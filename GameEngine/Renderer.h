#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <vector>
#include "glm/glm.hpp"
#include "Structs.h"

namespace engine
{

	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, bool flipX = false, bool flipY = false) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, bool flipX = false, bool flipY = false) const;
		void RenderTexture(const Texture2D& texture, const structs::Rect& dstRect, bool flipX = false, bool flipY = false) const;
		void RenderTexture(const Texture2D& texture, const structs::Rect& dstRect, const structs::Rect& srcRect, bool flipX = false, bool flipY = false) const;
		void RenderTexture(const Texture2D& texture, float x, float y, const structs::Rect& srcRect, bool flipX = false, bool flipY = false) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		glm::vec2 GetWindowSize()const;

	private:
		friend class Singleton<Renderer>;
		Renderer() = default;

		SDL_Rect GetSDLDstRect(const Texture2D& texture, float x, float y, float width, float height)const;
		SDL_Rect GetSDLSrcRect(const Texture2D& texture, float x, float y, float width, float height)const;

		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};
		bool m_ShowImGuiDemo{ false };
	};

}