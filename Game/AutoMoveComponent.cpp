#include "AutoMoveComponent.h"
#include "GameObject.h"
#include "TimeManager.h"

using namespace engine;

void AutoMoveComponent::Update()
{
	GetTransform()->Translate(m_Direction * m_Speed * TimeManager::GetInstance().GetDeltaTime());
}
