#pragma once
#include "BaseComponent.h"
#include "InputManager.h"

class PlayerController : public BaseComponent
{
public:
	PlayerController(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~PlayerController() = default;

	virtual void Initialize() override {};
	virtual void Update() override {};

	void BindKeyboardButtonToCommand(SDL_Scancode scanCode, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command);
	void BindControllerButtonToCommand(InputController::ControllerButton button, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command);

	//only works for bindings that are added after calling this function
	void UseKeyboard(bool use); 
	//set to -1 to no longer use controller
	//only works for bindings that are added after calling this function
	void UseController(int controllerIndex);

private:
	int m_ControllerIndex{ -1 };
	bool m_UseKeyboard{};
};
