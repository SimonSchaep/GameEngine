#pragma once
#include "TextRenderComponent.h"
#include "Observer.h"

class PlayerPoints;

class PointsDisplay : public engine::TextRenderComponent, engine::Observer<>
{
public:
	PointsDisplay(engine::GameObject* pGameObject);
	virtual ~PointsDisplay() = default;
	PointsDisplay(const PointsDisplay& other) = delete;
	PointsDisplay(PointsDisplay&& other) = delete;
	PointsDisplay& operator=(const PointsDisplay& other) = delete;
	PointsDisplay& operator=(PointsDisplay&& other) = delete;

	void SetSubjectGameObject(engine::GameObject* pGameObject);

	virtual void Notify() override;

private:
	PlayerPoints* m_pPlayerPoints{};
	int m_Points{};
};

