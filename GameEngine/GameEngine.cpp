#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GameEngine.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "TextureRenderComponent.h"
#include "CollisionManager.h"
#include "ServiceLocator.h"
#include <iostream>
#include <chrono>

namespace engine
{

	SDL_Window* g_window{};

	void PrintSDLVersion()
	{
		SDL_version version{};
		SDL_VERSION(&version);
		printf("We compiled against SDL version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

		SDL_GetVersion(&version);
		printf("We are linking against SDL version %u.%u.%u.\n",
			version.major, version.minor, version.patch);

		SDL_IMAGE_VERSION(&version);
		printf("We compiled against SDL_image version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

		version = *IMG_Linked_Version();
		printf("We are linking against SDL_image version %u.%u.%u.\n",
			version.major, version.minor, version.patch);

		SDL_TTF_VERSION(&version)
			printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
				version.major, version.minor, version.patch);

		version = *TTF_Linked_Version();
		printf("We are linking against SDL_ttf version %u.%u.%u.\n",
			version.major, version.minor, version.patch);
	}

	GameEngine::GameEngine(const std::string& dataPath)
	{
		PrintSDLVersion();

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
		}

		g_window = SDL_CreateWindow(
			"Programming 4 exam",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			960,
			720,
			SDL_WINDOW_OPENGL
		);
		if (g_window == nullptr)
		{
			throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
		}

		Renderer::GetInstance().Init(g_window);

		ResourceManager::GetInstance().Init(dataPath);

		m_InitSucces = true;
	}

	GameEngine::~GameEngine()
	{
		Renderer::GetInstance().Destroy();
		SceneManager::GetInstance().ClearScenes(); //clear before exiting since some objects might still do things in destructor
		SDL_DestroyWindow(g_window);
		g_window = nullptr;
		SDL_Quit();
	}

	void GameEngine::Run(const std::function<void()>& load)
	{
		if (!m_InitSucces) return;

		load();

		auto& renderer = Renderer::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = TimeManager::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();

		bool doContinue = true;

		sceneManager.Initialize();

		while (doContinue)
		{
			//INPUT
			doContinue = input.ProcessInput();

			//TIME
			time.Update(); //calculate deltatime, totaltime..., will sleep to cap fps when a cap is set

			//UPDATE + RENDER
			sceneManager.Update();
			CollisionManager::GetInstance().CheckTriggers();
			renderer.Render();
		}
	}
}