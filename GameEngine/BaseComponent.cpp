#include "BaseComponent.h"
#include "GameObject.h"

BaseComponent::BaseComponent(GameObject* gameObject)
	: m_pGameObject{ gameObject }
{
}
