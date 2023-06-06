#include "PlayerController.h"
#include "InputController.h"

namespace engine
{
	PlayerController::~PlayerController()
	{
		for (auto pCommand : m_BaseCommands)
		{
			InputManager::GetInstance().RemoveCommand(pCommand);
		}
		for (auto pAxisCommand : m_BaseAxisCommands)
		{
			InputManager::GetInstance().RemoveCommand(pAxisCommand);
		}
	}

	void PlayerController::BindKeyboardButtonToCommand(SDL_Scancode scanCode, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command)
	{
		if (m_UseKeyboard)
		{
			m_BaseCommands.emplace_back(InputManager::GetInstance().BindKeyboardButtonToCommand(scanCode, keyState, std::move(command)));
		}
	}

	void PlayerController::BindControllerButtonToCommand(InputController::ControllerButton button, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command)
	{
		if (m_ControllerIndex >= 0)
		{
			m_BaseCommands.emplace_back(InputManager::GetInstance().BindControllerButtonToCommand(m_ControllerIndex, button, keyState, std::move(command)));
		}
	}

	void PlayerController::BindControllerAxisToCommand(InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command)
	{
		if (m_ControllerIndex >= 0)
		{
			m_BaseAxisCommands.emplace_back(InputManager::GetInstance().BindControllerAxisToCommand(m_ControllerIndex, axis, std::move(command)));
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
}