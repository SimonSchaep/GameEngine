#pragma once
#include "TextRenderComponent.h"
#include "Observer.h"

using namespace engine;

class PlayerPoints;

class PointsDisplay : public TextRenderComponent, Observer
{
public:
	PointsDisplay(GameObject* pGameObject);
	virtual ~PointsDisplay() = default;
	PointsDisplay(const PointsDisplay& other) = delete;
	PointsDisplay(PointsDisplay&& other) = delete;
	PointsDisplay& operator=(const PointsDisplay& other) = delete;
	PointsDisplay& operator=(PointsDisplay&& other) = delete;

	void SetSubjectGameObject(GameObject* pGameObject);

	virtual void Notify(Event* pEvent, GameObject* pGameObject) override;

private:
	PlayerPoints* m_pPlayerPoints{};
	int m_Points{};
};

