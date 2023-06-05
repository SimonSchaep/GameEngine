#include "BaseComponent.h"
#include "GameObject.h"
#include <iostream>

namespace engine
{
	void BaseComponent::InitializeComponent()
	{
		if (m_IsInitialized) return;
		Initialize();
		m_IsInitialized = true;
	}
	void BaseComponent::UpdateComponent()
	{
		if (!m_IsInitialized)
		{
			InitializeComponent();
		}
		Update();
	}
	Transform* BaseComponent::GetTransform() const
	{
		return GetGameObject()->GetTransform();
	}
	BaseComponent::BaseComponent(GameObject* gameObject)
		: m_pGameObject{ gameObject }
	{
	}

}