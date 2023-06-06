#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl2.h"
#include "imgui.h"

namespace engine
{

	bool InputManager::ProcessInput()
	{
		ProcessControllerInput();
		ProcessKeyboardInput();

		return ProcessSDLEvents();
	}

	void InputManager::AddController()
	{
		m_Controllers.emplace_back(std::make_unique<InputController>(int(m_Controllers.size())));
	}

	void InputManager::RemoveController(int controllerIndex)
	{
		m_Controllers.erase(std::remove(m_Controllers.begin(), m_Controllers.end(), m_Controllers[controllerIndex]), m_Controllers.end());
	}

	BaseCommand* InputManager::BindKeyboardButtonToCommand(SDL_Scancode scanCode, KeyState keyState, std::unique_ptr<BaseCommand> command)
	{
		auto pReturnValue = command.get();
		if (m_KeyboardInputBindings.find(KeyboardKey{scanCode,keyState}) != m_KeyboardInputBindings.end())
		{
			m_KeyboardInputBindings[KeyboardKey{scanCode, keyState}].emplace_back(std::move(command));
		}
		else
		{
			std::vector<std::unique_ptr<BaseCommand>> commandVec{};
			commandVec.emplace_back(std::move(command));
			m_KeyboardInputBindings.emplace(KeyboardKey{ scanCode,keyState }, std::move(commandVec));
		}
		return pReturnValue;
	}

	BaseCommand* InputManager::BindControllerButtonToCommand(int controllerIndex, InputController::ControllerButton button, KeyState keyState, std::unique_ptr<BaseCommand> command)
	{
		auto pReturnValue = command.get();
		if (m_ControllerButtonBindings.find(ControllerButtonKey{ controllerIndex, button, keyState }) != m_ControllerButtonBindings.end())
		{
			m_ControllerButtonBindings[ControllerButtonKey{ controllerIndex, button, keyState }].emplace_back(std::move(command));
		}
		else
		{
			std::vector<std::unique_ptr<BaseCommand>> commandVec{};
			commandVec.emplace_back(std::move(command));
			m_ControllerButtonBindings.emplace(ControllerButtonKey{ controllerIndex, button, keyState }, std::move(commandVec));
		}
		return pReturnValue;
	}

	BaseAxisCommand* InputManager::BindControllerAxisToCommand(int controllerIndex, InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command)
	{
		auto pReturnValue = command.get();
		if (m_ControllerAxisBindings.find(ControllerAxisKey{ controllerIndex, axis }) != m_ControllerAxisBindings.end())
		{
			m_ControllerAxisBindings[ControllerAxisKey{ controllerIndex, axis }].emplace_back(std::move(command));
		}
		else
		{
			std::vector<std::unique_ptr<BaseAxisCommand>> commandVec{};
			commandVec.emplace_back(std::move(command));
			m_ControllerAxisBindings.emplace(ControllerAxisKey{ controllerIndex, axis }, std::move(commandVec));
		}
		return pReturnValue;
	}

	void InputManager::RemoveCommand(BaseCommand* baseCommand)
	{
		for (auto& [key, value] : m_KeyboardInputBindings)
		{
			auto it = std::find_if(value.begin(), value.end(), [&](std::unique_ptr<BaseCommand>& element) { return element.get() == baseCommand; });
			if (it != value.end())
			{
				value.erase(it);
				return;
			}
		}

		for (auto& [key, value] : m_ControllerButtonBindings)
		{
			auto it = std::find_if(value.begin(), value.end(), [&](std::unique_ptr<BaseCommand>& element) { return element.get() == baseCommand; });
			if (it != value.end())
			{
				value.erase(it);
				return;
			}
		}
	}

	void InputManager::RemoveCommand(BaseAxisCommand* baseAxisCommand)
	{
		for (auto& [key, value] : m_ControllerAxisBindings)
		{
			auto it = std::find_if(value.begin(), value.end(), [&](std::unique_ptr<BaseAxisCommand>& element) { return element.get() == baseAxisCommand; });
			if (it != value.end())
			{
				value.erase(it);
				return;
			}
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
				const InputController::ControllerButton controllerButton = controllerKey.controllerButton;

				//check if controllerindex is the same as the controllerindex that was bound to the command
				if (controllerKey.controllerIndex != m_Controllers[i]->GetControllerIndex())
				{
					continue;
				}

				switch (controllerKey.keyState)
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
				const InputController::ControllerAxis controllerAxis = controllerKey.controllerAxis;

				//check if controllerindex is the same as the controllerindex that was bound to the command
				if (controllerKey.controllerIndex != m_Controllers[i]->GetControllerIndex())
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
			if (keyboardKey.keyState != KeyState::pressed)
			{
				continue;
			}
			if (keys[keyboardKey.scanCode])
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
					if (keyboardKey.keyState != KeyState::down)
					{
						continue;
					}
					if (e.key.keysym.scancode == keyboardKey.scanCode)
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
					if (keyboardKey.keyState != KeyState::up)
					{
						continue;
					}
					if (e.key.keysym.scancode == keyboardKey.scanCode)
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

}