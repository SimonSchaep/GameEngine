#include "LivesDisplay.h"
#include <SDL_ttf.h>
#include "PlayerLives.h"
#include "GameObject.h"
#include <iostream>
#include "CustomEvents.h"

LivesDisplay::LivesDisplay(GameObject* pGameObject)
	:TextRenderComponent(pGameObject)
{
	SetColor({ 255,0,0,255 });
	SetText("Lives: 0");
}

void LivesDisplay::SetSubjectGameObject(GameObject* pGameObject)
{
	m_pPlayerLives = pGameObject->GetComponent<PlayerLives>();

	assert(m_pPlayerLives);

	m_pPlayerLives->GetDieEvent()->AddObserver(this);
	m_pPlayerLives->GetResetLivesEvent()->AddObserver(this);
}

void LivesDisplay::Notify(Event*, GameObject*)
{
	SetText("Lives: " + std::to_string(m_pPlayerLives->GetLives()));
}
