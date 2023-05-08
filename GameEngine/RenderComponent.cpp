#include "RenderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

engine::RenderComponent::RenderComponent(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	pGameObject->RegisterRenderComponent(this);
}
