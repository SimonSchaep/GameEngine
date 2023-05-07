#include "Transform.h"
#include "GameObject.h"

namespace engine
{

	Transform::Transform(GameObject* pGameObject)
		: m_pGameObject{ pGameObject }
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

	void Transform::SetLocalPosition(const glm::vec2& pos)
	{
		glm::vec3 temp{ pos.x, pos.y, 0 };
		m_LocalPosition = temp;

		SetDirty(true);
	}

	void Transform::SetLocalPosition(const glm::vec3& pos)
	{
		m_LocalPosition = pos;

		SetDirty(true);
	}

	void Transform::SetWorldPosition(float x, float y, float z)
	{
		if (m_pGameObject->GetParent())
		{
			SetLocalPosition(x - m_pGameObject->GetParent()->GetTransform()->GetWorldPosition().x,
				y - m_pGameObject->GetParent()->GetTransform()->GetWorldPosition().y,
				z - m_pGameObject->GetParent()->GetTransform()->GetWorldPosition().z);
		}
		else
		{
			SetLocalPosition(x, y, z);
		}
	}

	void Transform::SetWorldPosition(const glm::vec2& pos)
	{
		if (m_pGameObject->GetParent())
		{
			SetLocalPosition(pos - glm::vec2(m_pGameObject->GetParent()->GetTransform()->GetWorldPosition()));
		}
		else
		{
			SetLocalPosition(pos);
		}
	}

	void Transform::SetWorldPosition(const glm::vec3& pos)
	{
		if (m_pGameObject->GetParent())
		{
			SetLocalPosition(pos - m_pGameObject->GetParent()->GetTransform()->GetWorldPosition());
		}
		else
		{
			SetLocalPosition(pos);
		}
	}

	void Transform::Translate(const glm::vec2& offset)
	{
		glm::vec3 temp{ offset.x, offset.y, 0 };
		m_LocalPosition += temp;

		SetDirty(true);
	}

	void Transform::Translate(const glm::vec3& offset)
	{
		m_LocalPosition += offset;

		SetDirty(true);
	}


}