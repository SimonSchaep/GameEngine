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
#include "TextureRenderComponent.h"
#include "FPSCounter.h"
#include "MoveComponent.h"
#include "RotateComponent.h"
#include "DestroyComponent.h"
#include "TrashTheCacheComponent.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	//background
	auto pGameObject = scene.CreateAndAddGameObject();
	auto pRenderComponent = pGameObject->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("background.tga");

	//logo
	pGameObject = scene.CreateAndAddGameObject();
	pRenderComponent = pGameObject->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("logo.tga");
	pGameObject->GetTransform()->SetLocalPosition(216, 180, 0);

	//title
	pGameObject = scene.CreateAndAddGameObject();
	auto textRenderComponent = pGameObject->CreateAndAddComponent<TextRenderComponent>();
	textRenderComponent->SetText("Programming 4 Assignment");
	textRenderComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	
	pGameObject->GetTransform()->SetLocalPosition(80, 20, 0);

	//fps counter gameobject
	pGameObject = scene.CreateAndAddGameObject();
	textRenderComponent = pGameObject->CreateAndAddComponent<TextRenderComponent>();
	textRenderComponent->SetText("fps test");
	textRenderComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 12));
	pGameObject->CreateAndAddComponent<FPSCounter>();
	pGameObject->GetTransform()->SetLocalPosition(20, 20, 0);

	//chef gameobject
	auto pChef = scene.CreateAndAddGameObject();
	pRenderComponent = pChef->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("chef.png");
	auto pRotateComponent = pChef->CreateAndAddComponent<RotateComponent>();
	pRotateComponent->SetOffset({ 250,250,0 });
	pRotateComponent->SetRadius(50);
	pRotateComponent->SetRotateSpeed(3);
	/*auto testDestroyComponent = pChef->CreateAndAddComponent<DestroyComponent>();
	testDestroyComponent->SetDestroyChildren(true);
	testDestroyComponent->SetLifeTime(1.0f);*/
	
	//bean gameobject
	auto pBean = scene.CreateAndAddGameObject();
	pRenderComponent = pBean->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("bean.png");
	pRotateComponent = pBean->CreateAndAddComponent<RotateComponent>();
	pRotateComponent->SetOffset({ 0,0,0 });
	pRotateComponent->SetRadius(30);
	pRotateComponent->SetRotateSpeed(-3);
	pBean->SetParent(pChef, false);


	//trash the cache
	auto pExercise1 = scene.CreateAndAddGameObject();
	pExercise1->CreateAndAddComponent<TrashTheCacheComponent>();
}

int main(int, char*[]) {
	GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}