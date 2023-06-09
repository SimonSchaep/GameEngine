#include "BaseComponent.h"
#include "GameObject.h"
#include <iostream>

namespace engine
{
	BaseComponent::~BaseComponent()
	{
		OnDisable();
	}
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
	Scene* BaseComponent::GetScene() const
	{
		return GetGameObject()->GetScene();
	}
	bool BaseComponent::IsActive()
	{
		return m_IsActive && GetGameObject()->IsActive();
	}
	BaseComponent::BaseComponent(GameObject* gameObject)
		: m_pGameObject{ gameObject }
	{
		OnEnable();
	}

}