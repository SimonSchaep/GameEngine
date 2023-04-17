#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include <string>
#include "SteamAchievements.h"

class Achievement;

class AchievementManager final : public Singleton<AchievementManager>
{
public:
	template<typename T>
	T* CreateAndAddAchievement();

	bool UnlockAchievement(const std::string& idName);

	//call this for steam achievements to work
	void InitializeSteamAchievements();

private:
	std::vector<std::unique_ptr<Achievement>> m_Achievements;

	//STEAM ACHIEVMENTS
	std::unique_ptr<CSteamAchievements> m_CSteamAchievements{};

	int m_NextId{};

	// Achievement array which will hold data about the achievements and their state
	std::vector<Achievement_t> m_SteamAchievements{};
};

template<typename T>
T* AchievementManager::CreateAndAddAchievement()
{
	auto achievement = std::make_unique<T>();
	T* pReturnValue = achievement.get();
	m_Achievements.push_back(std::move(achievement));
	return pReturnValue;
}
