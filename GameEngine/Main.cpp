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
#include "RotateComponent.h"

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
	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 12);
	textRenderComponent = std::make_unique<TextRenderComponent>(go.get(), "fps test", font);
	go->AddComponent<TextRenderComponent>(std::move(textRenderComponent));
	auto fpsCounter = std::make_unique<FPSCounter>(go.get());
	go->AddComponent<FPSCounter>(std::move(fpsCounter));
	go->SetLocalPosition(20, 20);
	scene.Add(std::move(go));

	//chef gameobject
	auto goChef = std::make_unique<GameObject>();
	renderComponent = std::make_unique<RenderComponent>(goChef.get());
	renderComponent->SetTexture("chef.png");
	goChef->AddComponent<RenderComponent>(std::move(renderComponent));
	auto rotateComponent = std::make_unique<RotateComponent>(goChef.get());
	rotateComponent->SetCenterPos({ 250,250,0 });
	rotateComponent->SetRadius(50);
	rotateComponent->SetRotateSpeed(180);
	goChef->AddComponent<RotateComponent>(std::move(rotateComponent));
	
	//bean gameobject
	auto goBean = std::make_unique<GameObject>();
	renderComponent = std::make_unique<RenderComponent>(goBean.get());
	renderComponent->SetTexture("bean.png");
	goBean->AddComponent<RenderComponent>(std::move(renderComponent));
	rotateComponent = std::make_unique<RotateComponent>(goBean.get());
	rotateComponent->SetCenterPos({ 0,0,0 });
	rotateComponent->SetRadius(30);
	rotateComponent->SetRotateSpeed(-180);
	goBean->AddComponent<RotateComponent>(std::move(rotateComponent));
	goBean->SetParent(goChef.get(), false);

	//add to scene
	scene.Add(std::move(goChef));
	scene.Add(std::move(goBean));
}

int main(int, char*[]) {
	GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}