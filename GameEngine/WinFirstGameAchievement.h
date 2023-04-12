#pragma once
#include "Achievement.h"

class WinFirstGameAchievement final : public Achievement
{
public:
	WinFirstGameAchievement() : Achievement(
		-1,
		"ACH_WIN_ONE_GAME",
		"Winner",
		"You won your first game!")
	{};
	virtual ~WinFirstGameAchievement() = default;

	virtual bool UnlockConditionMet(Event* pEvent, GameObject* pGameObject)override;

	virtual void AddSubjectGameObject(GameObject* pGameObject)override;

	void SetPointsTreshold(int treshold) { m_PointsTreshold = treshold; }
private:
	int m_PointsTreshold{500};
};