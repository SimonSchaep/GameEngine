#include <stdexcept>
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
		//CollisionManager::GetInstance().Render(); //todo: figure out why this messes up the UI
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

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
	{
		int windowHeight{};
		SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), nullptr, &windowHeight);

		SDL_Rect dst{};
		dst.x = static_cast<int>(x);
		dst.y = static_cast<int>(y);
		dst.y = windowHeight - dst.y;
		SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
		dst.y -= dst.h;
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
	}

	void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
	{
		int windowHeight{};
		SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), nullptr, &windowHeight);

		SDL_Rect dst{};
		dst.x = static_cast<int>(x);
		dst.y = static_cast<int>(y);
		dst.y = windowHeight - dst.y;
		dst.w = static_cast<int>(width);
		dst.h = static_cast<int>(height);
		dst.y -= dst.h;
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
	}

	inline SDL_Renderer* Renderer::GetSDLRenderer() const { return m_Renderer; }

	glm::vec2 Renderer::GetWindowSize() const
	{
		int width{};
		int height{};
		SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &width, &height);
		return glm::vec2(width, height);
	}
}