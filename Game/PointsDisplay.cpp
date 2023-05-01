#include "PointsDisplay.h"
#include "GameObject.h"
#include "PlayerPoints.h"
#include "Event.h"
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

void PointsDisplay::Notify()
{
	m_Points += 50;

	SetText("Points: " + m_Points);
}