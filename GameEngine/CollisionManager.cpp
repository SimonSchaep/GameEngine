#include "CollisionManager.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"

void engine::CollisionManager::CheckTriggers()
{
	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	const auto& gameObjects = pActiveScene->GetGameObjects();

	//todo: optimize this

	//for every collider
	for (size_t i{}; i < gameObjects.size(); ++i)
	{
		if (!gameObjects[i]->IsActive()) continue;

		for (auto& collider : gameObjects[i]->GetColliders())
		{
			if (!collider->IsActive()) continue;

			//check triggers with every other collider
			for (size_t j{i + 1}; j < gameObjects.size(); ++j) //start with i + 1 to make sure we don't check collisions twice
			{
				if (!gameObjects[j]->IsActive()) continue;

				for (auto& otherCollider : gameObjects[j]->GetColliders())
				{
					if (!otherCollider->IsActive()) continue;

					collider->CheckTrigger(otherCollider);
				}
			}
		}
	}
	
}

void engine::CollisionManager::Render() const
{
	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	const auto& gameObjects = pActiveScene->GetGameObjects();

	for (size_t i{}; i < gameObjects.size(); ++i)
	{
		if (!gameObjects[i]->IsActive()) continue;

		for (auto& collider : gameObjects[i]->GetColliders())
		{
			if (!collider->IsActive()) continue;

			BoxCollider* pBox = dynamic_cast<BoxCollider*>(collider);
			if (pBox)
			{
				auto shape = pBox->GetShape();
				shape.bottomLeft += glm::vec2(pBox->GetGameObject()->GetTransform()->GetWorldPosition().x, pBox->GetGameObject()->GetTransform()->GetWorldPosition().y);
				SDL_Rect rect{};
				rect.x = int(shape.bottomLeft.x);
				rect.y = int(Renderer::GetInstance().GetWindowSize().y) - int(shape.bottomLeft.y);
				rect.w = int(shape.width);
				rect.h = int(shape.height);
				rect.y -= rect.h;

				SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
				SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
			}
		}
	}
	
}
