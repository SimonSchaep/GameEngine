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

	void BindKeyboardButtonToCommand(SDL_Scancode scanCode, InputManager::KeyState keyState, std::unique_ptr<BaseButtonCommand> command);
	void BindControllerButtonToCommand(InputController::ControllerButton button, InputManager::KeyState keyState, std::unique_ptr<BaseButtonCommand> command);
	void BindControllerAxisToCommand(InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command);

	//only works for bindings that are added after calling this function
	void UseKeyboard(bool use); 
	//set to -1 to no longer use controller
	//only works for bindings that are added after calling this function
	void UseController(int controllerIndex);

private:
	int m_ControllerIndex{ -1 };
	bool m_UseKeyboard{};
};

