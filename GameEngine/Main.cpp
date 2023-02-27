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
#include "MoveComponent.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_unique<GameObject>();
	auto renderComponent = std::make_unique<RenderComponent>(go.get());
	renderComponent->SetTexture("background.tga");
	go->AddComponent<RenderComponent>(std::move(renderComponent));
	scene.Add(std::move(go));

	go = std::make_unique<GameObject>();
	renderComponent = std::make_unique<RenderComponent>(go.get());
	renderComponent->SetTexture("logo.tga");
	go->AddComponent<RenderComponent>(std::move(renderComponent));
	go->SetLocalPosition(216, 180);
	scene.Add(std::move(go));

	go = std::make_unique<GameObject>();
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textRenderComponent = std::make_unique<TextRenderComponent>(go.get(), "Programming 4 Assignment", font);
	go->AddComponent<TextRenderComponent>(std::move(textRenderComponent));
	go->SetLocalPosition(80, 20);
	scene.Add(std::move(go));

	//fps counter gameobject
	go = std::make_unique<GameObject>();
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textRenderComponent = std::make_unique<TextRenderComponent>(go.get(), "fps test", font);
	go->AddComponent<TextRenderComponent>(std::move(textRenderComponent));
	auto fpsCounter = std::make_unique<FPSCounter>(go.get());
	go->AddComponent<FPSCounter>(std::move(fpsCounter));
	auto moveComponent = std::make_unique<MoveComponent>(go.get());
	go->AddComponent<MoveComponent>(std::move(moveComponent));
	go->SetLocalPosition(80, 120);
	scene.Add(std::move(go));
}

int main(int, char*[]) {
	GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}