#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "GameEngine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "UITextRenderComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextureRenderComponent.h"
#include "SpriteRenderComponent.h"
#include "SpriteStateMachineComponent.h"
#include "SpriteState.h"
#include "Sprite.h"
#include "Font.h"
#include "FPSCounter.h"
#include "RotateComponent.h"
#include "DestroyComponent.h"
#include "TrashTheCacheComponent.h"
#include "MovementComponent.h"
#include "PlayerController.h"
#include "MyPlayerController.h"
#include "PlayerLives.h"
#include "LivesDisplay.h"
#include "PlayerPoints.h"
#include "PointsDisplay.h"
#include "ControlsDisplay.h"
#include "Level.h"
#include "Texture2D.h"
#include "BoxCollider.h"
#include "ColliderDebugRenderComponent.h"
#include <CameraComponent.h>
#include "ServiceLocator.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"
#include "GameManager.h"
#include "ChefSpriteController.h"
#include "ScoreManager.h"
#include "MuteCommand.h"

using namespace engine;

void load()
{
	//register services
	ServiceLocator::RegisterLogger<ConsoleLogger>();
	//logger->SetEnableTimestamps(true);
	/*auto logger = ServiceLocator::RegisterLogger<FileLogger>();
	logger->SetActualLogger<ConsoleLogger>();*/

	ServiceLocator::RegisterSoundSystem<SDLSoundSystem>();
	/*auto soundSystem = ServiceLocator::RegisterSoundSystem<LoggingSoundSystem>();
	soundSystem->SetActualSoundSystem<SDLSoundSystem>();*/


	auto pScene = SceneManager::GetInstance().CreateScene("EmptyScene", true);

	ServiceLocator::GetSoundSystem().SetGlobalVolume(10);

	//mute button
	InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_M, InputManager::KeyState::up, std::make_unique<MuteCommand>());


	//background
	//auto pGameObject = pScene->CreateAndAddGameObject("Background");
	//auto pRenderComponent = pGameObject->CreateAndAddComponent<TextureRenderComponent>();
	//pRenderComponent->SetTexture("background.tga");

	//logo
	//pGameObject = pScene->CreateAndAddGameObject("Logo");
	//pRenderComponent = pGameObject->CreateAndAddComponent<TextureRenderComponent>();
	//pRenderComponent->SetTexture("logo.tga");
	//pGameObject->GetTransform()->SetLocalPosition(216, 180, 0);

	//title
	//pGameObject = pScene->CreateAndAddGameObject("Title");
	//auto pTextRenderComponent = pGameObject->CreateAndAddComponent<TextRenderComponent>();
	//pTextRenderComponent->SetText("Programming 4 Assignment");
	//pTextRenderComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));

	//pGameObject->GetTransform()->SetLocalPosition(80, 20, 0);

	//fps counter gameobject
	auto pGameObject = pScene->CreateAndAddGameObject("Fps");
	pGameObject->MarkAsSceneIndependant();
	auto pTextRenderComponent = pGameObject->CreateAndAddComponent<UITextRenderComponent>();
	pTextRenderComponent->SetText("FPS:");
	pTextRenderComponent->SetFont("fonts/super-burger-time.ttf", 12);
	pGameObject->CreateAndAddComponent<FPSCounter>();
	pGameObject->GetTransform()->SetLocalPosition(20, 20, 0);


	//Load level
	//auto& level = Level::GetInstance();
	//level.BuildLevel(pScene, "Data/level2.csv");

	// separating gameobjects from controllers makes it possible to switch between controlled gameobjects
	// input will first go to a controller, then the controller will send the input to a controlled gameobject
	// you could put the logic of the MyPlayerController in the movementcomponent, or put the MyPlayerController on the same gameobject if you don't need the switching

	//chef gameobject
	//auto pChef = pScene->CreateAndAddGameObject("Chef");
	//pChef->AddTag("Chef");
	//pChef->GetTransform()->SetLocalPosition({ 96,596 });
	//auto pMovementComponent = pChef->CreateAndAddComponent<MovementComponent>();
	//pMovementComponent->SetMoveSpeed(100);
	//auto pPlayerLives = pChef->CreateAndAddComponent<PlayerLives>();
	//pPlayerLives->SetMaxLives(5);
	//pChef->CreateAndAddComponent<PlayerPoints>();	

	////visuals
	//auto pChefVisuals = pScene->CreateAndAddGameObject("ChefVisuals", pChef);
	///*auto pRenderComponent = pChefVisuals->CreateAndAddComponent<TextureRenderComponent>();
	//pRenderComponent->SetTexture("chef.png");
	//pRenderComponent->SetSize({28, 28});
	//float width = float(pRenderComponent->GetSize().x);
	//float height = float(pRenderComponent->GetSize().y);*/
	//auto pSpriteRenderComponent = pChefVisuals->CreateAndAddComponent<SpriteRenderComponent>();
	//pSpriteRenderComponent->SetSize({ 28, 28 });

	//pChefVisuals->CreateAndAddComponent<ChefSpriteController>();

	//float width = float(pSpriteRenderComponent->GetSize().x);
	//float height = float(pSpriteRenderComponent->GetSize().y);
	//pChefVisuals->GetTransform()->SetLocalPosition({ -width / 2, -8 });

	////collider
	//auto pBoxCollider = pChef->CreateAndAddComponent<BoxCollider>();
	//pBoxCollider->SetShape({ -width / 2, -8, width, height });

	//camera
	//auto pCamera = pChef->CreateAndAddComponent<CameraComponent>();
	//pScene->SetActiveCamera(pCamera);
	//pCamera->SetLevelBoundaries({ level.GetLevelPosition().x,level.GetLevelPosition().y,float(level.GetLevelWidth() * level.GetCellWidth()),float((level.GetLevelHeight() + 1) * level.GetCellHeight()) }); //make sure level is built before this

	//bean gameobject
	//auto pBean = pScene->CreateAndAddGameObject("Bean");
	//pBean->GetTransform()->SetLocalPosition({ 96,596 });
	//pMovementComponent = pBean->CreateAndAddComponent<MovementComponent>();
	//pMovementComponent->SetMoveSpeed(100);
	//pPlayerLives = pBean->CreateAndAddComponent<PlayerLives>();
	//pPlayerLives->SetMaxLives(5);
	//pBean->CreateAndAddComponent<PlayerPoints>();

	////visuals
	//auto pBeanVisuals = pScene->CreateAndAddGameObject("BeanVisuals", pBean);
	//auto pRenderComponent = pBeanVisuals->CreateAndAddComponent<TextureRenderComponent>();
	//pRenderComponent->SetTexture("bean.png");
	//pRenderComponent->SetSize({ 28, 28 });
	//width = float(pRenderComponent->GetSize().x);
	//height = float(pRenderComponent->GetSize().y);
	//pBeanVisuals->GetTransform()->SetLocalPosition({ -width / 2, -4 });

	////collider
	//pBoxCollider = pBean->CreateAndAddComponent<BoxCollider>();
	//pBoxCollider->SetShape({ -width / 2, -4, width, height });

	////player 1 controller gameobject
	//auto pPlayer1ControllerObject = pScene->CreateAndAddGameObject();
	//auto pPlayer1Controller = pPlayer1ControllerObject->CreateAndAddComponent<MyPlayerController>();
	//pPlayer1Controller->UseKeyboard(true);
	//pPlayer1Controller->UseController(1);
	////make controller posses chef
	//pPlayer1Controller->SetControlChef(true);

	//player 2 controller gameobject
	//auto pPlayer2ControllerObject = pScene->CreateAndAddGameObject();
	//auto pPlayer2Controller = pPlayer2ControllerObject->CreateAndAddComponent<MyPlayerController>();
	//pPlayer2Controller->UseController(0);
	////make controller posses bean
	//pPlayer2Controller->SetControlChef(false);

	//Observers
	/*auto pChefLivesDisplay = pScene->CreateAndAddGameObject("ChefLivesDisplay");
	pChefLivesDisplay->GetTransform()->SetLocalPosition({ 20,300 });
	auto pLivesDisplayComponent = pChefLivesDisplay->CreateAndAddComponent<LivesDisplay>();
	pLivesDisplayComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	pLivesDisplayComponent->SetColor({ 255,255,0,255 });
	pLivesDisplayComponent->SetSubjectGameObject(pChef);

	auto pBeanLivesDisplay = pScene->CreateAndAddGameObject("ChefLivesDisplay");
	pBeanLivesDisplay->GetTransform()->SetLocalPosition({ 20,400 });
	pLivesDisplayComponent = pBeanLivesDisplay->CreateAndAddComponent<LivesDisplay>();
	pLivesDisplayComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	pLivesDisplayComponent->SetColor({ 0,255,0,255 });
	pLivesDisplayComponent->SetSubjectGameObject(pBean);

	auto pChefPointsDisplay = pScene->CreateAndAddGameObject("ChefLivesDisplay");
	pChefPointsDisplay->GetTransform()->SetLocalPosition({ 20,340 });
	auto pPointsDisplayComponent = pChefPointsDisplay->CreateAndAddComponent<PointsDisplay>();
	pPointsDisplayComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	pPointsDisplayComponent->SetColor({ 255,255,0,255 });
	pPointsDisplayComponent->SetSubjectGameObject(pChef);

	auto pBeanPointsDisplay = pScene->CreateAndAddGameObject("ChefLivesDisplay");
	pBeanPointsDisplay->GetTransform()->SetLocalPosition({ 20,440 });
	pPointsDisplayComponent = pBeanPointsDisplay->CreateAndAddComponent<PointsDisplay>();
	pPointsDisplayComponent->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 18));
	pPointsDisplayComponent->SetColor({ 0,255,0,255 });
	pPointsDisplayComponent->SetSubjectGameObject(pBean);*/


	//Controls display
	auto pControlsDisplay = pScene->CreateAndAddGameObject("ControlsDisplay");
	pControlsDisplay->MarkAsSceneIndependant();
	pControlsDisplay->CreateAndAddComponent<ControlsDisplay>();

	
	//Game Manager
	auto pGameManager = pScene->CreateAndAddGameObject("GameManager");
	pGameManager->MarkAsSceneIndependant();
	pGameManager->CreateAndAddComponent<GameManager>();
	pGameManager->CreateAndAddComponent<ScoreManager>("score/scores.txt");
}

int main(int, char* [])
{
	GameEngine engine("Data/");
	engine.Run(load);

	return 0;
}