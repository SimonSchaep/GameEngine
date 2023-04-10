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

	m_pPlayerPoints->GetAddPointsEvent()->AddObserver(this);
}

void PointsDisplay::Notify(Event*, GameObject*)
{
	if (!m_pPlayerPoints)
	{
		std::cerr << "Call SetSubjectGameObject first\n";
		return;
	}
	SetText("Points: " + std::to_string(m_pPlayerPoints->GetPoints()));
}