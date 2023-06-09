#include "LifeTimer.h"
#include "TimeManager.h"
#include "GameObject.h"

using namespace engine;

void LifeTimer::Update()
{
	m_LifeTimer -= TimeManager::GetInstance().GetDeltaTime();

	if (m_LifeTimer <= 0)
	{
		GetGameObject()->MarkForDeletion(true);
	}
}
