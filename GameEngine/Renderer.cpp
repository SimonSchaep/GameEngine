#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "CollisionManager.h" //collision debug rendering
#include "Texture2D.h"
#include "imgui.h"
#include "implot.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl2.h"
#include "SDL_opengl.h"
#include <iostream>
#include <stdexcept>

namespace engine
{

	int GetOpenGLDriverIndex()
	{
		auto openglIndex = -1;
		const auto driverCount = SDL_GetNumRenderDrivers();
		for (auto i = 0; i < driverCount; i++)
		{
			SDL_RendererInfo info;
			if (!SDL_GetRenderDriverInfo(i, &info))
				if (!strcmp(info.name, "opengl"))
					openglIndex = i;
		}
		return openglIndex;
	}

	void Renderer::Init(SDL_Window* window)
	{
		m_Window = window;
		m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
		if (m_Renderer == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
		ImGui_ImplOpenGL2_Init();
	}

	void Renderer::Render() const
	{
		const auto& color = GetBackgroundColor();
		SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderClear(m_Renderer);

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame(m_Window);
		ImGui::NewFrame();

		glPushMatrix();
		CameraComponent* pCamera = SceneManager::GetInstance().GetActiveScene()->GetActiveCamera();
		if (pCamera)
		{
			pCamera->Transform();
		}

		SceneManager::GetInstance().Render();

#ifdef _DEBUG
		//CollisionManager::GetInstance().Render(); //todo: figure out why this messes up imgui
#endif // DEBUG		

		glPopMatrix();

		SceneManager::GetInstance().RenderUI();

		if (m_ShowImGuiDemo)
		{
			ImGui::ShowDemoWindow();
		}
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		SDL_RenderPresent(m_Renderer);
	}

	void Renderer::Destroy()
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		if (m_Renderer != nullptr)
		{
			SDL_DestroyRenderer(m_Renderer);
			m_Renderer = nullptr;
		}
	}

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, bool flipX, bool flipY) const
	{
		RenderTexture(texture, x, y, 0, 0, flipX, flipY);
	}

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, bool flipX, bool flipY) const
	{
		auto dst = GetSDLDstRect(texture, x, y, width, height);

		if (flipX)
		{
			glPushMatrix();
			glTranslatef(dst.x + dst.w / 2.f, dst.y + dst.h / 2.f, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-(dst.x + dst.w / 2.f), -(dst.y + dst.h / 2.f), 0);
		}
		if (flipY)
		{
			glPushMatrix();
			glTranslatef(dst.x + dst.w / 2.f, dst.y + dst.h / 2.f, 0);
			glScalef(1, -1, 1);
			glTranslatef(-(dst.x + dst.w / 2.f), -(dst.y + dst.h / 2.f), 0);
		}

		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);

		if (flipX)
		{
			glPopMatrix();
		}
		if (flipY)
		{
			glPopMatrix();
		}
	}

	void Renderer::RenderTexture(const Texture2D& texture, const structs::Rect& dstRect, bool flipX, bool flipY) const
	{
		RenderTexture(texture, dstRect.bottomLeft.x, dstRect.bottomLeft.y, dstRect.width, dstRect.height, flipX, flipY);
	}

	void Renderer::RenderTexture(const Texture2D& texture, const structs::Rect& dstRect, const structs::Rect& srcRect, bool flipX, bool flipY) const
	{
		auto dst = GetSDLDstRect(texture, dstRect.bottomLeft.x, dstRect.bottomLeft.y, dstRect.width, dstRect.height);

		auto src = GetSDLSrcRect(texture, srcRect.bottomLeft.x, srcRect.bottomLeft.y, srcRect.width, srcRect.height);

		if (flipX)
		{
			glPushMatrix();
			glTranslatef(dst.x + dst.w / 2.f, dst.y + dst.h / 2.f, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-(dst.x + dst.w / 2.f), -(dst.y + dst.h / 2.f), 0);
		}
		if (flipY)
		{
			glPushMatrix();
			glTranslatef(dst.x + dst.w / 2.f, dst.y + dst.h / 2.f, 0);
			glScalef(1, -1, 1);
			glTranslatef(-(dst.x + dst.w / 2.f), -(dst.y + dst.h / 2.f), 0);
		}

		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);

		if (flipX)
		{
			glPopMatrix();
		}
		if (flipY)
		{
			glPopMatrix();
		}
	}

	void Renderer::RenderTexture(const Texture2D& texture, float x, float y, const structs::Rect& srcRect, bool flipX, bool flipY) const
	{
		RenderTexture(texture, { x,y,0,0 }, srcRect, flipX, flipY);
	}

	inline SDL_Renderer* Renderer::GetSDLRenderer() const { return m_Renderer; }

	glm::vec2 Renderer::GetWindowSize() const
	{
		int width{};
		int height{};
		SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
		return glm::vec2(width, height);
	}
	SDL_Rect Renderer::GetSDLDstRect(const Texture2D& texture, float x, float y, float width, float height) const
	{
		int windowHeight{};
		SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), nullptr, &windowHeight);

		SDL_Rect dst{};
		dst.x = static_cast<int>(x);
		dst.y = static_cast<int>(y);
		dst.y = windowHeight - dst.y;

		if (width == 0 || height == 0)
		{
			SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
		}
		else
		{
			dst.w = static_cast<int>(width);
			dst.h = static_cast<int>(height);
		}	

		dst.y -= dst.h;

		return dst;
	}
	SDL_Rect Renderer::GetSDLSrcRect(const Texture2D& texture, float x, float y, float width, float height) const
	{
		SDL_Rect src{};
		src.x = static_cast<int>(x);
		src.y = static_cast<int>(y);

		if (width == 0 || height == 0)
		{
			SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &src.w, &src.h);
		}
		else
		{
			src.w = static_cast<int>(width);
			src.h = static_cast<int>(height);
		}

		src.y -= src.h;

		return src;
	}
}