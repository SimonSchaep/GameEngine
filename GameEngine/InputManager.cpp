#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl2.h"
#include "imgui.h"

bool InputManager::ProcessInput()
{
	for (size_t i{}; i < m_Controllers.size(); ++i)
	{
		m_Controllers[i]->Update();

		for (auto& pair : m_ControllerInputBindings)
		{
			const ControllerKey& controllerKey = pair.first;
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
					for (auto& command : pair.second)
					{
						command->Execute();
					}
				}
				break;
			case KeyState::up:
				if (m_Controllers[i]->isUp(controllerButton))
				{
					for (auto& command : pair.second)
					{
						command->Execute();
					}
				}
				break;
			case KeyState::pressed:
				if (m_Controllers[i]->isPressed(controllerButton))
				{
					for (auto& command : pair.second)
					{
						command->Execute();
					}
				}
				break;
			}
		}
	}	

	auto keys = SDL_GetKeyboardState(nullptr);

	for (auto& pair : m_KeyboardInputBindings)
	{
		const KeyboardKey& keyboardKey = pair.first;
		if (keyboardKey.second != KeyState::pressed)
		{
			continue;
		}
		if (keys[keyboardKey.first])
		{
			for (auto& command : pair.second)
			{
				command->Execute();
			}
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) 
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN) //this down works different to controller down
		{
			for (auto& pair : m_KeyboardInputBindings)
			{
				const KeyboardKey& keyboardKey = pair.first;
				if (keyboardKey.second != KeyState::down)
				{
					continue;
				}
				if (e.key.keysym.scancode == keyboardKey.first)
				{
					for (auto& command : pair.second)
					{
						command->Execute();
					}
				}
			}
		}
		if (e.type == SDL_KEYUP)
		{
			for (auto& pair : m_KeyboardInputBindings)
			{
				const KeyboardKey& keyboardKey = pair.first;
				if (keyboardKey.second != KeyState::up)
				{
					continue;
				}
				if (e.key.keysym.scancode == keyboardKey.first)
				{
					for (auto& command : pair.second)
					{
						command->Execute();
					}
				}
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) 
		{
			
		}
		// process event for imgui
		ImGui_ImplSDL2_ProcessEvent(&e);
		// etc...
	}

	return true;
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
	if (m_ControllerInputBindings.find(std::pair(std::pair(controllerIndex, button), keyState)) != m_ControllerInputBindings.end())
	{
		m_ControllerInputBindings[std::pair(std::pair(controllerIndex, button), keyState)].push_back(std::move(command));
	}
	else
	{
		std::vector<std::unique_ptr<BaseCommand>> commandVec{};
		commandVec.push_back(std::move(command));
		m_ControllerInputBindings.emplace(std::pair(std::pair(controllerIndex, button), keyState), std::move(commandVec));
	}
}
