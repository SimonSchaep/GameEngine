#include "Transform.h"
#include "GameObject.h"

Transform::Transform(GameObject* pGameObject)
	: m_pGameObject{pGameObject}
{
}

void Transform::SetDirty(bool dirty)
{
	m_IsDirty = dirty;

	for (auto child : m_pGameObject->GetChildren())
	{
		child->GetTransform()->SetDirty(dirty);
	}
}

const glm::vec3& Transform::GetWorldPosition()
{
	if (m_IsDirty)
	{
		if (m_pGameObject->GetParent())
		{
			m_Worldposition = m_pGameObject->GetParent()->GetTransform()->GetWorldPosition() + m_LocalPosition;
		}
		else
		{
			m_Worldposition = m_LocalPosition;
		}
		m_IsDirty = false;
	}
	return m_Worldposition;
}

void Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	
	SetDirty(true);
}

void Transform::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;

	SetDirty(true);
}
