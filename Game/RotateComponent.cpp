#include "RotateComponent.h"
#include "GameObject.h"
#include "TimeManager.h"

void RotateComponent::Initialize()
{
}

void RotateComponent::Update()
{
	m_Angle += TimeManager::GetInstance().GetDeltaTime() * m_RotateSpeed;

	GetGameObject()->GetTransform()->SetLocalPosition({ m_Radius * cosf(m_Angle), m_Radius * sinf(m_Angle) });
}
