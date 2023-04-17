#pragma once
#include "Observer.h"
#include <string>

class GameObject;

class Achievement : public Observer
{
public:
	Achievement(int id, const std::string& idName, const std::string& name, const std::string& description);
	virtual ~Achievement() = default;

	//define the unlock condition here
	virtual bool UnlockConditionMet(Event* pEvent, GameObject* pGameObject) = 0;

	//add the achievement as an observer here
	virtual void AddSubjectGameObject(GameObject* pGameObject) = 0;

	//don't overwrite this
	virtual void Notify(Event* pEvent, GameObject* pGameObject)override;

	int GetId() { return m_Id; }
	const std::string& GetIdName() { return m_IdName; }
	const std::string& GetName() { return m_Name; }
	const std::string& GetDescription() { return m_Description; }
	bool IsAchieved() { return m_Achieved; };

private:
	void Unlock();

	bool m_Achieved{};
	int m_Id{};
	std::string m_IdName{};
	std::string m_Name{};
	std::string m_Description{};
	
};