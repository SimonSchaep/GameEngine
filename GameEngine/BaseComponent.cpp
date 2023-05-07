#include "BaseComponent.h"
#include "GameObject.h"

namespace engine
{
	void BaseComponent::InitializeComponent()
	{		
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
	BaseComponent::BaseComponent(GameObject* gameObject)
		: m_pGameObject{ gameObject }
	{
	}

}