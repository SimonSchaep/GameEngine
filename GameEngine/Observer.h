#pragma once
class Event;
class GameObject;

class Observer
{
public:
	Observer() = default;
	virtual ~Observer() = default;

	virtual void Notify(Event* pEvent, GameObject* pGameObject) = 0;
};

