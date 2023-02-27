#include "Transform.h"

void Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Transform::SetPosition(const glm::vec3& pos)
{
	m_position = pos;
}
