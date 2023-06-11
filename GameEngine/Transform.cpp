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

	const glm::vec2& Transform::GetWorldPosition()
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

	void Transform::SetLocalPosition(float x, float y)
	{
		SetLocalPosition({ x,y });

		SetDirty(true);
	}

	void Transform::SetLocalPosition(const glm::vec2& pos)
	{
		m_LocalPosition = pos;

		SetDirty(true);
	}

	void Transform::SetWorldPosition(float x, float y)
	{
		SetWorldPosition({ x,y });
	}

	void Transform::SetWorldPosition(const glm::vec2& pos)
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
		m_LocalPosition += offset;

		SetDirty(true);
	}

	void Transform::Translate(float x, float y)
	{
		m_LocalPosition += glm::vec2{ x,y };

		SetDirty(true);
	}


}