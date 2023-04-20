#include "BaseComponent.h"
#include "GameObject.h"

namespace engine
{

	BaseComponent::BaseComponent(GameObject* gameObject)
		: m_pGameObject{ gameObject }
	{
	}

}