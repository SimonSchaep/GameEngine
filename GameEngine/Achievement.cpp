#include "Achievement.h"
#include "AchievementManager.h"

Achievement::Achievement(int id, const std::string& idName, const std::string& name, const std::string& description)
	:m_Id{id}
	, m_IdName{idName}
	, m_Name{ name }
	, m_Description{description}
{
}

void Achievement::Notify(Event* pEvent, GameObject* pGameObject)
{
	if (UnlockConditionMet(pEvent, pGameObject))
	{
		Unlock();
	}
}

void Achievement::Unlock()
{
	AchievementManager::GetInstance().UnlockAchievement(m_IdName);
}
