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
#include "RotateComponent.h"
#include "DestroyComponent.h"
#include "TrashTheCacheComponent.h"
#include "MovementComponent.h"
#include "PlayerController.h"
#include "PlayerMovementController.h"

void load()
{
	auto pScene = SceneManager::GetInstance().CreateScene("Demo");

	//background
	auto pGameObject = pScene->CreateAndAddGameObject("Background");
	auto pRenderComponent = pGameObject->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("background.tga");

	//logo
	pGameObject = pScene->CreateAndAddGameObject("Logo");
	pRenderComponent = pGameObject->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("logo.tga");
	pGameObject->GetTransform()->SetLocalPosition(216, 180, 0);

	//title
	pGameObject = pScene->CreateAndAddGameObject("Title");
	auto textRenderComponent = pGameObject->CreateAndAddComponent<TextRenderComponent>();
	textRenderComponent->SetText("Programming 4 Assignment");
	textRenderComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
	
	pGameObject->GetTransform()->SetLocalPosition(80, 20, 0);

	//fps counter gameobject
	pGameObject = pScene->CreateAndAddGameObject("Fps");
	textRenderComponent = pGameObject->CreateAndAddComponent<TextRenderComponent>();
	textRenderComponent->SetText("fps test");
	textRenderComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 12));
	pGameObject->CreateAndAddComponent<FPSCounter>();
	pGameObject->GetTransform()->SetLocalPosition(20, 20, 0);

	//chef gameobject
	auto pChef = pScene->CreateAndAddGameObject("Chef");
	pChef->GetTransform()->SetLocalPosition({100,100});
	pRenderComponent = pChef->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("chef.png");
	auto pMovementComponent = pChef->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(50);
	auto pChefController = pScene->CreateAndAddGameObject();
	auto pPlayerMovementController = pChefController->CreateAndAddComponent<PlayerMovementController>();
	pPlayerMovementController->UseKeyboard(true);
	pPlayerMovementController->UseController(1);
	pPlayerMovementController->SetControlChef(true);

	/*auto testDestroyComponent = pChef->CreateAndAddComponent<DestroyComponent>();
	testDestroyComponent->SetDestroyChildren(true);
	testDestroyComponent->SetLifeTime(1.0f);*/
	
	//bean gameobject
	auto pBean = pScene->CreateAndAddGameObject("Bean");
	pBean->GetTransform()->SetLocalPosition({ 100,100 });
	pRenderComponent = pBean->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture("bean.png");
	pMovementComponent = pBean->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(100);
	auto pBeanController = pScene->CreateAndAddGameObject();
	pPlayerMovementController = pBeanController->CreateAndAddComponent<PlayerMovementController>();
	pPlayerMovementController->UseController(0);
	pPlayerMovementController->SetControlChef(false);
	
	/*auto pRotateComponent = pBean->CreateAndAddComponent<RotateComponent>();
	pRotateComponent->SetRadius(30);
	pRotateComponent->SetRotateSpeed(-3);
	pBean->SetParent(pChef, false);*/


	//trash the cache
	//auto pTrashTheCache = pScene.CreateAndAddGameObject();
	//pTrashTheCache->CreateAndAddComponent<TrashTheCacheComponent>();
}

int main(int, char*[]) {
	GameEngine engine("../Data/");
	engine.Run(load);
    return 0;
}