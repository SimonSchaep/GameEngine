#include "RenderComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

engine::RenderComponent::RenderComponent(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	Scene* pScene = SceneManager::GetInstance().GetSceneByIndex(pGameObject->GetSceneIndex());
	if (pScene)
	{
		pScene->AddRenderComponent(this);
	}
	else
	{
		std::cout << "scene related to index " << pGameObject->GetSceneIndex() << " not found\n";
	}
}
