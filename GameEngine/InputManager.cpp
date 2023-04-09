#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl2.h"
#include "imgui.h"

bool InputManager::ProcessInput()
{
	ProcessControllerInput();
	ProcessKeyboardInput();
	
	return ProcessSDLEvents();	
}

void InputManager::AddController()
{
	m_Controllers.push_back(std::make_unique<InputController>(int(m_Controllers.size())));
}

void InputManager::RemoveController(int controllerIndex)
{
	m_Controllers.erase(std::remove(m_Controllers.begin(), m_Controllers.end(), m_Controllers[controllerIndex]), m_Controllers.end());
}

void InputManager::BindKeyboardButtonToCommand(SDL_Scancode scanCode, KeyState keyState, std::unique_ptr<BaseCommand> command)
{
	if (m_KeyboardInputBindings.find(std::pair(scanCode, keyState)) != m_KeyboardInputBindings.end())
	{
		m_KeyboardInputBindings[std::pair(scanCode, keyState)].push_back(std::move(command));
	}
	else
	{
		std::vector<std::unique_ptr<BaseCommand>> commandVec{};
		commandVec.push_back(std::move(command));
		m_KeyboardInputBindings.emplace(std::pair(scanCode, keyState), std::move(commandVec));
	}
}

void InputManager::BindControllerButtonToCommand(int controllerIndex, InputController::ControllerButton button, KeyState keyState, std::unique_ptr<BaseCommand> command)
{
	if (m_ControllerButtonBindings.find(std::pair(std::pair(controllerIndex, button), keyState)) != m_ControllerButtonBindings.end())
	{
		m_ControllerButtonBindings[std::pair(std::pair(controllerIndex, button), keyState)].push_back(std::move(command));
	}
	else
	{
		std::vector<std::unique_ptr<BaseCommand>> commandVec{};
		commandVec.push_back(std::move(command));
		m_ControllerButtonBindings.emplace(std::pair(std::pair(controllerIndex, button), keyState), std::move(commandVec));
	}
}

void InputManager::BindControllerAxisToCommand(int controllerIndex, InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command)
{
	if (m_ControllerAxisBindings.find(std::pair(controllerIndex, axis)) != m_ControllerAxisBindings.end())
	{
		m_ControllerAxisBindings[std::pair(controllerIndex, axis)].push_back(std::move(command));
	}
	else
	{
		std::vector<std::unique_ptr<BaseAxisCommand>> commandVec{};
		commandVec.push_back(std::move(command));
		m_ControllerAxisBindings.emplace(std::pair(controllerIndex, axis), std::move(commandVec));
	}
}


void InputManager::ProcessControllerInput()
{
	for (size_t i{}; i < m_Controllers.size(); ++i)
	{
		m_Controllers[i]->Update();

		//BUTTONS
		for (auto& binding : m_ControllerButtonBindings)
		{
			const ControllerButtonKey& controllerKey = binding.first;
			const InputController::ControllerButton controllerButton = controllerKey.first.second;

			//check if controllerindex is the same as the controllerindex that was bound to the command
			if (controllerKey.first.first != m_Controllers[i]->GetControllerIndex())
			{
				continue;
			}

			switch (controllerKey.second)
			{
			case KeyState::down:
				if (m_Controllers[i]->isDown(controllerButton))
				{
					for (auto& command : binding.second)
					{
						command->Execute();
					}
				}
				break;
			case KeyState::up:
				if (m_Controllers[i]->isUp(controllerButton))
				{
					for (auto& command : binding.second)
					{
						command->Execute();
					}
				}
				break;
			case KeyState::pressed:
				if (m_Controllers[i]->isPressed(controllerButton))
				{
					for (auto& command : binding.second)
					{
						command->Execute();
					}
				}
				break;
			}
		}

		//AXIS
		for (auto& binding : m_ControllerAxisBindings)
		{
			const ControllerAxisKey& controllerKey = binding.first;
			const InputController::ControllerAxis controllerAxis = controllerKey.second;

			//check if controllerindex is the same as the controllerindex that was bound to the command
			if (controllerKey.first != m_Controllers[i]->GetControllerIndex())
			{
				continue;
			}

			for (auto& command : binding.second)
			{
				command->Execute(m_Controllers[i]->GetAxis(controllerAxis));
			}
		}
	}
}

void InputManager::ProcessKeyboardInput()
{
	auto keys = SDL_GetKeyboardState(nullptr);

	for (auto& binding : m_KeyboardInputBindings)
	{
		const KeyboardKey& keyboardKey = binding.first;
		if (keyboardKey.second != KeyState::pressed)
		{
			continue;
		}
		if (keys[keyboardKey.first])
		{
			for (auto& command : binding.second)
			{
				command->Execute();
			}
		}
	}
}

bool InputManager::ProcessSDLEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN) //this down works different to controller down
		{
			for (auto& binding : m_KeyboardInputBindings)
			{
				const KeyboardKey& keyboardKey = binding.first;
				if (keyboardKey.second != KeyState::down)
				{
					continue;
				}
				if (e.key.keysym.scancode == keyboardKey.first)
				{
					for (auto& command : binding.second)
					{
						command->Execute();
					}
				}
			}
		}
		if (e.type == SDL_KEYUP)
		{
			for (auto& binding : m_KeyboardInputBindings)
			{
				const KeyboardKey& keyboardKey = binding.first;
				if (keyboardKey.second != KeyState::up)
				{
					continue;
				}
				if (e.key.keysym.scancode == keyboardKey.first)
				{
					for (auto& command : binding.second)
					{
						command->Execute();
					}
				}
			}
		}
		// process event for imgui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}