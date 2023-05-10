#include "CollisionManager.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"

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

void engine::CollisionManager::Render() const
{
	auto pActiveScene = SceneManager::GetInstance().GetActiveScene();
	const std::vector<std::shared_ptr<GameObject>>& gameObjects = pActiveScene->GetGameObjects();

	for (size_t i{}; i < gameObjects.size(); ++i)
	{
		for (auto& collider : gameObjects[i]->GetColliders())
		{
			BoxCollider* pBox = dynamic_cast<BoxCollider*>(collider);
			if (pBox)
			{
				auto shape = pBox->GetShape();
				shape.BottomLeft += glm::vec2(pBox->GetGameObject()->GetTransform()->GetWorldPosition().x, pBox->GetGameObject()->GetTransform()->GetWorldPosition().y);
				SDL_Rect rect{};
				rect.x = int(shape.BottomLeft.x);
				rect.y = int(Renderer::GetInstance().GetWindowSize().y) - int(shape.BottomLeft.y);
				rect.w = int(shape.Width);
				rect.h = int(shape.Height);
				rect.y -= rect.h;

				SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
				SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
			}
		}
	}
	
}
