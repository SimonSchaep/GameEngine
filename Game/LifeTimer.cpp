#include "LifeTimer.h"
#include "TimeManager.h"
#include "GameObject.h"

void LifeTimer::Update()
{
	m_LifeTimer -= engine::TimeManager::GetInstance().GetDeltaTime();

	if (m_LifeTimer <= 0)
	{
		GetGameObject()->MarkForDeletion(true);
	}
}
