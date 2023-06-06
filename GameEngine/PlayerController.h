#pragma once
#include "BaseComponent.h"
#include "InputManager.h"

namespace engine
{

	class PlayerController : public BaseComponent
	{
	public:
		PlayerController(GameObject* pGameObject) :BaseComponent(pGameObject) {};
		virtual ~PlayerController();

		virtual void Initialize() override {};
		virtual void Update() override {};

		void BindKeyboardButtonToCommand(SDL_Scancode scanCode, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command);
		void BindControllerButtonToCommand(InputController::ControllerButton button, InputManager::KeyState keyState, std::unique_ptr<BaseCommand> command);
		void BindControllerAxisToCommand(InputController::ControllerAxis axis, std::unique_ptr<BaseAxisCommand> command);

		//only works for bindings that are added after calling this function
		void UseKeyboard(bool useKeyboard);
		//set to -1 to no longer use controller
		//only works for bindings that are added after calling this function
		void UseController(int controllerIndex);

	private:
		int m_ControllerIndex{ -1 };
		bool m_UseKeyboard{};

		//store so we can remove them when this controller gets destroyed
		std::vector<BaseCommand*> m_BaseCommands{};
		std::vector<BaseAxisCommand*> m_BaseAxisCommands{};
	};

}