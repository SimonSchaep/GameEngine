#include "BaseComponent.h"
#include "GameObject.h"
#include <iostream>

namespace engine
{
	void BaseComponent::InitializeComponent()
	{	
		//std::cout << GetGameObject()->GetName() << "init\n";
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