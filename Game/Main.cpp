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
#include "UITextRenderComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "Font.h"
#include "FPSCounter.h"
#include "ServiceLocator.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"
#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"
#include "GameManager.h"
#include "ScoreManager.h"
#include "MuteCommand.h"
#include "InputManager.h"
#include "ControlsDisplay.h"

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
	InputManager::GetInstance().BindKeyboardButtonToCommand(SDL_SCANCODE_F3, InputManager::KeyState::up, std::make_unique<MuteCommand>());

	//fps counter gameobject
	auto pGameObject = pScene->CreateAndAddGameObject("Fps");
	pGameObject->MarkAsSceneIndependant();
	auto pTextRenderComponent = pGameObject->CreateAndAddComponent<UITextRenderComponent>();
	pTextRenderComponent->SetText("FPS:");
	pTextRenderComponent->SetFont("fonts/super-burger-time.ttf", 12);
	pGameObject->CreateAndAddComponent<FPSCounter>();
	pGameObject->GetTransform()->SetLocalPosition(20, 20);

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