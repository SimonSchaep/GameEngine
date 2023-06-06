#include "UIRenderComponent.h"
#include "GameObject.h"

namespace engine
{
	UIRenderComponent::UIRenderComponent(GameObject* pGameObject)
		:BaseComponent(pGameObject)
	{
		pGameObject->RegisterUIRenderComponent(this);
	}
	UIRenderComponent::~UIRenderComponent()
	{
		GetGameObject()->RemoveUIRenderComponent(this);
	}
}