#pragma once
#include "TextRenderComponent.h"
#include "Observer.h"

using namespace engine;

class PlayerLives;

class LivesDisplay : public TextRenderComponent, Observer
{
public:
	LivesDisplay(GameObject* pGameObject);
	virtual ~LivesDisplay() = default;
	LivesDisplay(const LivesDisplay& other) = delete;
	LivesDisplay(LivesDisplay&& other) = delete;
	LivesDisplay& operator=(const LivesDisplay& other) = delete;
	LivesDisplay& operator=(LivesDisplay&& other) = delete;

	void SetSubjectGameObject(GameObject* pGameObject);

	virtual void Notify(Event* pEvent, GameObject* pGameObject) override;

private:
	PlayerLives* m_pPlayerLives{};
};

