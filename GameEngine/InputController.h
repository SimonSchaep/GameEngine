#pragma once
#include <memory>

namespace engine
{

	class InputController final
	{
	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000,
		};

		enum class ControllerAxis
		{
			LeftTrigger,
			RightTrigger,
			ThumbLX,
			ThumbLY,
			ThumbRX,
			ThumbRY,
		};

		void Update();

		bool isDown(ControllerButton button)const;
		bool isUp(ControllerButton button)const;
		bool isPressed(ControllerButton button)const;

		float GetAxis(ControllerAxis axis)const;

		int GetControllerIndex()const;

		InputController(int controllerIndex);
		~InputController();

	private:
		class InputControllerImpl;
		std::unique_ptr<InputControllerImpl> m_Impl;
	};

}