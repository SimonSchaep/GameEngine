#pragma once
#include "Singleton.h"
#include "InputController.h"
#include "BaseCommand.h"
#include <SDL_scancode.h> //for sdl scancodes
#include <unordered_map>
#include <map>
#include <vector>

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
	bool IsPressed(int button) { return button; };
	void BindKeyboardButtonToCommand(SDL_Scancode scanCode, KeyState keyState, std::unique_ptr<BaseCommand> command);
	void BindControllerButtonToCommand(int controllerIndex, InputController::ControllerButton button, KeyState keyState, std::unique_ptr<BaseCommand> command);

private:
	using ControllerKey = std::pair<std::pair<int, InputController::ControllerButton>, KeyState>;
	using ControllerCommandsMap = std::map<ControllerKey, std::vector<std::unique_ptr<BaseCommand>>>;
	ControllerCommandsMap m_ControllerInputBindings{};
	std::vector<std::unique_ptr<InputController>> m_Controllers;

	using KeyboardKey = std::pair<SDL_Scancode, KeyState>;
	std::map<KeyboardKey, std::vector<std::unique_ptr<BaseCommand>>> m_KeyboardInputBindings{};
};