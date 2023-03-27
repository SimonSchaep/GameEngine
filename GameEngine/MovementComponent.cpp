#include "MovementComponent.h"
#include "MoveCommands.h"
#include "GameObject.h"
#include "Time.h"
#include "InputManager.h"
#include "InputController.h"
#include "PlayerController.h"


void MovementComponent::Update()
{
	if (m_Direction != glm::vec2{0, 0})
	{
		float magnitude = glm::length(m_Direction);
		if (magnitude > 1)
		{
			m_Direction /= magnitude;
		}

		GetGameObject()->GetTransform()->Translate(m_Direction * m_MoveSpeed * Time::GetInstance().GetDeltaTime());

		m_Direction = { 0,0 };
	}
}

void MovementComponent::Move(const glm::vec2& direction)
{
	m_Direction += direction;
}


