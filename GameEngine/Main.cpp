#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "GameEngine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRenderComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "FPSCounter.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	auto renderComponent = std::make_shared<dae::RenderComponent>();
	renderComponent->SetTexture("background.tga");
	go->AddComponent<dae::RenderComponent>(renderComponent);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	renderComponent = std::make_shared<dae::RenderComponent>();
	renderComponent->SetTexture("logo.tga");
	go->AddComponent<dae::RenderComponent>(renderComponent);
	go->SetPosition(216, 180);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textRenderComponent= std::make_shared<dae::TextRenderComponent>("Programming 4 Assignment", font);
	go->AddComponent<dae::TextRenderComponent>(textRenderComponent);
	go->SetPosition(80, 20);
	scene.Add(go);

	//fps counter gameobject
	go = std::make_shared<dae::GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textRenderComponent = std::make_shared<dae::TextRenderComponent>("fps test", font);
	go->AddComponent<dae::TextRenderComponent>(textRenderComponent);
	auto fpsCounter = std::make_shared<dae::FPSCounter>();
	go->AddComponent<dae::FPSCounter>(fpsCounter);
	go->SetPosition(80, 120);
	scene.Add(go);
}

int main(int, char*[]) {
	dae::GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}