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
	for (size_t i{}; i < gameObjects.size(); ++i)
	{
		for (auto& collider : gameObjects[i]->GetColliders())
		{			
			//check triggers with every other collider
			for (size_t j{i + 1}; j < gameObjects.size(); ++j) //start with i + 1 to make sure we don't check collisions twice
			{
				for (auto& otherCollider : gameObjects[j]->GetColliders())
				{
					collider->CheckTrigger(otherCollider);
				}
			}
		}
	}
	
}
