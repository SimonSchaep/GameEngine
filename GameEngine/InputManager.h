#pragma once
#include "Singleton.h"
#include "InputController.h"
#include "BaseCommand.h"
#include "BaseAxisCommand.h"
#include <SDL_scancode.h> //for sdl scancodes
#include <unordered_map>
#include <map>
#include <vector>

namespace engine
{

	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class KeyState
		{
			down,
			up,
			pressed,
		};

		void AddController();
		void RemoveController(int controllerIndex);

		bool ProcessInput();

		void BindControllerButtonToCommand(int controllerIndex, InputController::ControllerButton button, KeyState keyState, std::unique_ptr<BaseCommand> command);
		void BindControllerAxisToCommand(int controllerIndex, InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command);
		void BindKeyboardButtonToCommand(SDL_Scancode scanCode, KeyState keyState, std::unique_ptr<BaseCommand> command);

	private:
		void ProcessControllerInput();
		void ProcessKeyboardInput();
		bool ProcessSDLEvents();

		std::vector<std::unique_ptr<InputController>> m_Controllers;

		using ControllerButtonKey = std::pair<std::pair<int, InputController::ControllerButton>, KeyState>;
		using ControllerButtonCommandsMap = std::map<ControllerButtonKey, std::vector<std::unique_ptr<BaseCommand>>>;
		ControllerButtonCommandsMap m_ControllerButtonBindings{};

		using ControllerAxisKey = std::pair<int, InputController::ControllerAxis>;
		using ControllerAxisCommandsMap = std::map<ControllerAxisKey, std::vector<std::unique_ptr<BaseAxisCommand>>>;
		ControllerAxisCommandsMap m_ControllerAxisBindings{};

		using KeyboardKey = std::pair<SDL_Scancode, KeyState>;
		using KeyboardCommandsMap = std::map<KeyboardKey, std::vector<std::unique_ptr<BaseCommand>>>;
		KeyboardCommandsMap m_KeyboardInputBindings{};
	};

}