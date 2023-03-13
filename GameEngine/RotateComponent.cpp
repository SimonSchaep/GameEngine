#include "RotateComponent.h"
#include "GameObject.h"
#include "Time.h"

void RotateComponent::Initialize()
{
}

void RotateComponent::Update()
{
	const float dt{ Time::GetInstance().GetDeltaTime() };
	m_Angle += dt * m_RotateSpeed;

	glm::vec3 pos{ m_Offset };
	
	pos.x += m_Radius * cosf(m_Angle);
	pos.y += m_Radius * sinf(m_Angle);
	GetGameObject()->GetTransform()->SetLocalPosition(pos);
}
