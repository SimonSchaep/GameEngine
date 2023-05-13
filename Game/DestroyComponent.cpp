#include "DestroyComponent.h"
#include "TimeManager.h"
#include "GameObject.h"

using namespace engine;

void DestroyComponent::Update()
{
	if (TimeManager::GetInstance().GetTotalTime() >= m_LifeTime)
	{
		GetGameObject()->MarkForDeletion(m_DestroyChildren);
	}
}
