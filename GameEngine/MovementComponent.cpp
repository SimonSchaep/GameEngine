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
		m_Direction = glm::normalize(m_Direction);

		GetGameObject()->GetTransform()->Translate(m_Direction * m_MoveSpeed * Time::GetInstance().GetDeltaTime());

		m_Direction = { 0,0 };
	}
}

void MovementComponent::Move(const glm::vec2& direction)
{
	m_Direction += direction;
}


