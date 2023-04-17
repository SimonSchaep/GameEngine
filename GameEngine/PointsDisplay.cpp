#include "PointsDisplay.h"
#include "GameObject.h"
#include "PlayerPoints.h"
#include "CustomEvents.h"
#include <iostream>

PointsDisplay::PointsDisplay(GameObject* pGameObject)
	:TextRenderComponent(pGameObject)
{
	SetColor({ 255,0,0,255 });
	SetText("Points: 0");
}

void PointsDisplay::SetSubjectGameObject(GameObject* pGameObject)
{
	m_pPlayerPoints = pGameObject->GetComponent<PlayerPoints>();

	assert(m_pPlayerPoints);

	m_pPlayerPoints->GetKilledEnemyEvent()->AddObserver(this);
}

void PointsDisplay::Notify(Event* pEvent, GameObject*)
{
	if (dynamic_cast<KilledEnemyEvent*>(pEvent))
	{
		m_Points += 50;
	}

	SetText("Points: " + m_Points);
}