#pragma once
#include "BaseComponent.h"
#include <glm/glm.hpp>


class AutoMoveComponent final : public engine::BaseComponent
{
public:
	AutoMoveComponent(engine::GameObject* pGameObject, float speed, const glm::vec2& direction) 
		:engine::BaseComponent(pGameObject) 
		,m_Speed{speed}
		,m_Direction{direction}
	{}
	virtual ~AutoMoveComponent() = default;

	virtual void Update()override;

private:
	float m_Speed{};
	glm::vec2 m_Direction{};
};

