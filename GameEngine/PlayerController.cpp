#include "PlayerController.h"
#include "InputController.h"

void PlayerController::BindKeyboardButtonToCommand(SDL_Scancode scanCode, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command)
{
	if (m_UseKeyboard)
	{
		InputManager::GetInstance().BindKeyboardButtonToCommand(scanCode, keyState, std::move(command));
	}
}

void PlayerController::BindControllerButtonToCommand(InputController::ControllerButton button, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command)
{
	if (m_ControllerIndex >= 0)
	{
		InputManager::GetInstance().BindControllerButtonToCommand(m_ControllerIndex, button, keyState, std::move(command));
	}
}

void PlayerController::BindControllerAxisToCommand(InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command)
{
	if (m_ControllerIndex >= 0)
	{
		InputManager::GetInstance().BindControllerAxisToCommand(m_ControllerIndex, axis, std::move(command));
	}
}

void PlayerController::UseKeyboard(bool useKeyboard)
{
	m_UseKeyboard = useKeyboard;
}

void PlayerController::UseController(int controllerIndex)
{
	if (m_ControllerIndex == -1)
	{
		InputManager::GetInstance().AddController();
	}
	if (controllerIndex == -1)
	{
		InputManager::GetInstance().RemoveController(m_ControllerIndex);
	}
	m_ControllerIndex = controllerIndex;
	
}
