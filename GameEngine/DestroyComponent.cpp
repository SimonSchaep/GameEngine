#include "DestroyComponent.h"
#include "Time.h"
#include "GameObject.h"

void DestroyComponent::Update()
{
	if (Time::GetInstance().GetTotalTime() >= m_LifeTime)
	{
		GetGameObject()->MarkForDeletion(m_DestroyChildren);
	}
}
