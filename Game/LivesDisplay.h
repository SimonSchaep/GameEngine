#pragma once
#include "TextRenderComponent.h"
#include "Observer.h"

class PlayerLives;

class LivesDisplay : public engine::TextRenderComponent, engine::Observer<>
{
public:
	LivesDisplay(engine::GameObject* pGameObject);
	virtual ~LivesDisplay() = default;
	LivesDisplay(const LivesDisplay& other) = delete;
	LivesDisplay(LivesDisplay&& other) = delete;
	LivesDisplay& operator=(const LivesDisplay& other) = delete;
	LivesDisplay& operator=(LivesDisplay&& other) = delete;

	void SetSubjectGameObject(engine::GameObject* pGameObject);

	void Notify() override;

private:
	PlayerLives* m_pPlayerLives{};
};

