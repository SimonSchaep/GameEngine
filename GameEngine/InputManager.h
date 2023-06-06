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

		BaseCommand* BindControllerButtonToCommand(int controllerIndex, InputController::ControllerButton button, KeyState keyState, std::unique_ptr<BaseCommand> command);
		BaseAxisCommand* BindControllerAxisToCommand(int controllerIndex, InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command);
		BaseCommand* BindKeyboardButtonToCommand(SDL_Scancode scanCode, KeyState keyState, std::unique_ptr<BaseCommand> command);

		void RemoveCommand(BaseCommand* baseCommand);
		void RemoveCommand(BaseAxisCommand* baseAxisCommand);

	private:
		friend class Singleton<InputManager>;
		InputManager() = default;

		void ProcessControllerInput();
		void ProcessKeyboardInput();
		bool ProcessSDLEvents();

		std::vector<std::unique_ptr<InputController>> m_Controllers;

		struct ControllerButtonKey
		{
			int controllerIndex{};
			InputController::ControllerButton controllerButton{};
			KeyState keyState{};

			auto operator<=>(const ControllerButtonKey&) const = default;
		};
		using ControllerButtonCommandsMap = std::map<ControllerButtonKey, std::vector<std::unique_ptr<BaseCommand>>>;
		ControllerButtonCommandsMap m_ControllerButtonBindings{};

		struct ControllerAxisKey
		{
			int controllerIndex{};
			InputController::ControllerAxis controllerAxis{};

			auto operator<=>(const ControllerAxisKey&) const = default;
		};
		using ControllerAxisCommandsMap = std::map<ControllerAxisKey, std::vector<std::unique_ptr<BaseAxisCommand>>>;
		ControllerAxisCommandsMap m_ControllerAxisBindings{};

		struct KeyboardKey
		{
			SDL_Scancode scanCode{};
			KeyState keyState{};

			auto operator<=>(const KeyboardKey&) const = default;
		};
		using KeyboardCommandsMap = std::map<KeyboardKey, std::vector<std::unique_ptr<BaseCommand>>>;
		KeyboardCommandsMap m_KeyboardInputBindings{};
	};

}