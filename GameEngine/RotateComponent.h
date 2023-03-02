#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"

class RotateComponent : public BaseComponent
{
public:
	RotateComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
	virtual ~RotateComponent() = default;

	virtual void Initialize() override;
	virtual void Update() override;

	void SetCenterPos(const glm::vec3& pos) { m_CenterPos = pos; };
	void SetRotateSpeed(float rotateSpeed) { m_RotateSpeed = rotateSpeed; };
	void SetRadius(float radius) { m_Radius = radius; };

private:
	glm::vec3 m_CenterPos{};
	float m_Angle{}; //degrees
	float m_RotateSpeed{30}; //degrees per second
	float m_Radius{30};
};

