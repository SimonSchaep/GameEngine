#include "InputController.h"
#include <Windows.h>
#include <Xinput.h>

class InputController::InputControllerImpl
{
public:
	InputControllerImpl(int controllerIndex)
		:m_ControllerIndex{controllerIndex}
	{
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	}
	~InputControllerImpl() = default;

	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned int button) const { return m_ButtonsPressedThisFrame & button; };
	bool IsUpThisFrame(unsigned int button) const { return m_ButtonsReleasedThisFrame & button; };
	bool IsPressed(unsigned int button) const { return m_CurrentState.Gamepad.wButtons & button; };

	float GetAxis(ControllerAxis axis)const
	{
		float value{};

		//todo: figure out better way to get these values
		const float maxValueThumb{ 32768 };
		const float maxValueTrigger{ 255 };

		switch (axis)
		{
		case InputController::ControllerAxis::LeftTrigger:
			value = m_CurrentState.Gamepad.bLeftTrigger / maxValueTrigger;
			break;
		case InputController::ControllerAxis::RightTrigger:
			value = m_CurrentState.Gamepad.bRightTrigger / maxValueTrigger;
			break;
		case InputController::ControllerAxis::ThumbLX:
			value = m_CurrentState.Gamepad.sThumbLX / maxValueThumb;
			break;
		case InputController::ControllerAxis::ThumbLY:
			value = m_CurrentState.Gamepad.sThumbLY / maxValueThumb;
			break;
		case InputController::ControllerAxis::ThumbRX:
			value = m_CurrentState.Gamepad.sThumbRX / maxValueThumb;
			break;
		case InputController::ControllerAxis::ThumbRY:
			value = m_CurrentState.Gamepad.sThumbRY / maxValueThumb;
			break;
		}

		const float deadZonePercentage = 0.05f;
		if (abs(value) < deadZonePercentage)
		{
			value = 0;
		}

		return value;
	}

	int GetControllerIndex()const { return m_ControllerIndex; };

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};

	WORD m_ButtonsPressedThisFrame;
	WORD m_ButtonsReleasedThisFrame;

	int m_ControllerIndex;
};

void InputController::Update()
{
	m_Impl->Update();
}

bool InputController::isDown(ControllerButton button) const
{
	return m_Impl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool InputController::isUp(ControllerButton button) const
{
	return m_Impl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool InputController::isPressed(ControllerButton button) const
{
	return m_Impl->IsPressed(static_cast<unsigned int>(button));
}

float InputController::GetAxis(ControllerAxis axis) const
{
	return m_Impl->GetAxis(axis);
}

int InputController::GetControllerIndex() const
{
	return m_Impl->GetControllerIndex();
}

InputController::InputController(int controllerIndex)
{
	m_Impl = std::make_unique<InputControllerImpl>(controllerIndex);
}


//https://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile
//implementing the destructor here instead of the .h file, makes the impl work with unique_ptr
//I think because when the default destructor is made it needs to know the implementation of all the objects in unique_ptrs
InputController::~InputController() = default;

