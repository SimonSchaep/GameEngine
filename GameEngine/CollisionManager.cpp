#include "CollisionManager.h"
#include "Collider.h"
#include "SceneManager.h"
#include "Scene.h"

void engine::CollisionManager::CheckTriggers()
{
	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = pActiveScene->GetGameObjects();

	//todo: optimize this

	//for every collider
	for (auto& gameObject : gameObjects)
	{
		for (auto& collider : gameObject->GetColliders())
		{
			//check triggers with every other collider
			for (auto& otherGameObject : gameObjects)
			{
				for (auto& otherCollider : otherGameObject->GetColliders())
				{
					collider->CheckTrigger(otherCollider);
				}
			}
		}
	}
	
}
