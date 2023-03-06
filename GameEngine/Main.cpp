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
	auto renderComponent = go->AddComponent<RenderComponent>();
	renderComponent->SetTexture("background.tga");
	scene.Add(std::move(go));

	go = std::make_unique<GameObject>();
	renderComponent = go->AddComponent<RenderComponent>();
	renderComponent->SetTexture("logo.tga");
	go->SetLocalPosition(216, 180);
	scene.Add(std::move(go));

	go = std::make_unique<GameObject>();
	auto textRenderComponent = go->AddComponent<TextRenderComponent>();
	textRenderComponent->SetText("Programming 4 Assignment");
	textRenderComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	
	go->SetLocalPosition(80, 20);
	scene.Add(std::move(go));

	//fps counter gameobject
	go = std::make_unique<GameObject>();
	textRenderComponent = go->AddComponent<TextRenderComponent>();
	textRenderComponent->SetText("fps test");
	textRenderComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 12));
	go->AddComponent<FPSCounter>();
	go->SetLocalPosition(20, 20);
	scene.Add(std::move(go));

	//chef gameobject
	auto goChef = std::make_unique<GameObject>();
	renderComponent = goChef->AddComponent<RenderComponent>();
	renderComponent->SetTexture("chef.png");
	auto rotateComponent = goChef->AddComponent<RotateComponent>();
	rotateComponent->SetCenterPos({ 250,250,0 });
	rotateComponent->SetRadius(50);
	rotateComponent->SetRotateSpeed(180);
	
	//bean gameobject
	auto goBean = std::make_unique<GameObject>();
	renderComponent = goBean->AddComponent<RenderComponent>();
	renderComponent->SetTexture("bean.png");
	rotateComponent = goBean->AddComponent<RotateComponent>();
	rotateComponent->SetCenterPos({ 0,0,0 });
	rotateComponent->SetRadius(30);
	rotateComponent->SetRotateSpeed(-180);
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