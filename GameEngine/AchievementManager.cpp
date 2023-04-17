#include "AchievementManager.h"
#include "Achievement.h"
#include <assert.h>

bool AchievementManager::UnlockAchievement(const std::string& idName)
{
	//make sure InitializeSteamAchievements was called
	assert(m_SteamAchievements.size() > 0);

	return m_CSteamAchievements->SetAchievement(idName.c_str());
}

void AchievementManager::InitializeSteamAchievements()
{
	m_SteamAchievements.clear();

	for (auto& achievement : m_Achievements)
	{
		int id{ achievement->GetId() };
		if (id == -1)
		{
			id = m_NextId;
		}
		m_NextId = id + 1;

		//would be cleaner to change the achievement_t member data types, but I don't want to mess with steam's code
		char name[128]{};
		char description[256]{};
		strcpy(name, achievement->GetName().c_str());
		strcpy(name, achievement->GetDescription().c_str());
		//description and name don't change anything right now, since it uses the name and desc from the game anyways, so no idea why this is even here
		m_SteamAchievements.push_back({id, achievement->GetIdName().c_str(), *name, *description, achievement->IsAchieved(), 0});
	}

	m_CSteamAchievements = std::make_unique<CSteamAchievements>(m_SteamAchievements.data(), int(m_SteamAchievements.size()));
}
