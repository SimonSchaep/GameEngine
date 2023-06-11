#include "Scene.h"
#include "RenderComponent.h"
#include "UIRenderComponent.h"
#include "CameraComponent.h"
#include <algorithm>

namespace engine
{
	Scene::Scene(const std::string& name, bool destroyAfterDisable) : m_Name(name), m_DestroyAfterDisable{ destroyAfterDisable } {}

	void Scene::SetActiveCamera(CameraComponent* pCameraComponent)
	{
		m_pCameraComponent = pCameraComponent;
	}

	CameraComponent* Scene::GetActiveCamera()
	{
		return m_pCameraComponent.Get();
	}

	Scene::~Scene() = default;

	GameObject* Scene::CreateAndAddGameObject(const std::string& name, GameObject* pParent)
	{
		auto gameObject = std::make_unique<GameObject>(name, this);
		if (pParent)
		{
			gameObject->SetParent(pParent, false);
		}
		GameObject* pReturnValue = gameObject.get();
		m_GameObjects.emplace_back(std::move(gameObject));
		return pReturnValue;
	}

	GameObject* Scene::FindGameObjectByName(const std::string& name)
	{
		for (auto& gameObject : m_GameObjects)
		{
			if (!gameObject->IsActive())continue;
			if (gameObject->GetName() == name)
			{
				return gameObject.get();
			}
		}
		return nullptr;
	}

	std::vector<GameObject*> Scene::FindAllGameObjectsWithName(const std::string& name)
	{
		std::vector<GameObject*> gameObjects{};
		for (auto& gameObject : m_GameObjects)
		{
			if (!gameObject->IsActive())continue;
			if (gameObject->GetName() == name)
			{
				gameObjects.emplace_back(gameObject.get());
			}
		}
		return gameObjects;
	}

	std::vector<GameObject*> Scene::FindAllGameObjectsWithTag(const std::string& tag)
	{
		std::vector<GameObject*> gameObjects{};
		for (auto& gameObject : m_GameObjects)
		{
			if (!gameObject->IsActive())continue;
			if (gameObject->HasTag(tag))
			{
				gameObjects.emplace_back(gameObject.get());
			}
		}
		return gameObjects;
	}

	void Scene::TransferSceneIndependantGameObjects(Scene* targetScene)
	{
		if(targetScene == this) return;

		for (auto& gameObject : m_GameObjects)
		{
			if (gameObject->IsSceneIndependant())
			{
				targetScene->TransferGameObject(std::move(gameObject));
			}
		}
		//moving unique pointers will leave empty ones in the vector
		//remove them here, after finishing iterating over them
		RemoveEmptyGameObjectUniquePointers();

		int i{};
		i++;
	}

	void Scene::TransferGameObject(std::unique_ptr<GameObject> gameObject)
	{
		m_GameObjects.emplace_back(std::move(gameObject));
		m_GameObjects.back()->OnSceneTransferred(this);
	}

	void Scene::Initialize()
	{
		if (m_IsInitialized) return;
		//no range-based for, cause then iterator would get invalidated if gameobjects get added during initailize
		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			m_GameObjects[i]->Initialize();
		}
		m_IsInitialized = true;
	}

	void Scene::Update()
	{
		//Update GameObjects
		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			if (!m_GameObjects[i]->IsActive())continue;

			if (m_GameObjects[i]->IsMarkedForDeletion())
			{
				m_ToDeleteIndexes.emplace_back(i);
			}
			else
			{
				m_GameObjects[i]->Update();
			}
		}

		//Delete GameObjects - reverse order because it removes gameobjects by index
		//If the gameobjects at idx 0 and 1 need to be deleted and the one at 0 is deleted first, the gameobject at idx 1 will be at 0 when we delete it
		for (int i{ int(m_ToDeleteIndexes.size()) - 1 }; i >= 0; --i)
		{
			RemoveGameObjectByIndex(m_ToDeleteIndexes[i]);
		}
		m_ToDeleteIndexes.clear();

		//maybe shoudln't do this every frame, but should be able to change layer at runtime
		SortGameObjectsByRenderLayer();
	}

	void Scene::Reset()
	{
		//no range-based for, cause then iterator would get invalidated if gameobjects get added during reset
		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			m_GameObjects[i]->Reset();
		}
	}

	void Scene::RenderUI()
	{
		for (const auto& gameObject : m_GameObjects)
		{
			if (!gameObject->IsActive())continue;

			gameObject->RenderUI();
		}
	}

	void Scene::Render()const
	{
		for (const auto& gameObject : m_GameObjects)
		{
			if (!gameObject->IsActive())continue;

			gameObject->Render();
		}
	}

	void Scene::SortGameObjectsByRenderLayer()
	{
		//Sort gameObjects so render will be based on layer of first rendercomponent of each gameobject
		std::sort(m_GameObjects.begin(), m_GameObjects.end(),
			[](const std::unique_ptr<GameObject>& obj1, const std::unique_ptr<GameObject>& obj2)
			{
				int layer1{};
				int layer2{};

				UIRenderComponent* pUIRenderComponent{ obj1->GetComponent<UIRenderComponent>() };
				if (pUIRenderComponent)
				{
					layer1 = pUIRenderComponent->GetLayer();
				}
				else
				{
					RenderComponent* pRenderComponent{ obj1->GetComponent<RenderComponent>() };
					if (pRenderComponent)
					{
						layer1 = pRenderComponent->GetLayer();
					}
				}				

				pUIRenderComponent = obj2->GetComponent<UIRenderComponent>();
				if (pUIRenderComponent)
				{
					layer2 = pUIRenderComponent->GetLayer();
				}
				else
				{
					RenderComponent* pRenderComponent{ obj2->GetComponent<RenderComponent>() };
					if (pRenderComponent)
					{
						layer2 = pRenderComponent->GetLayer();
					}
				}

				return (layer1 < layer2);
			});
	}

	void Scene::RemoveGameObjectByIndex(size_t i)
	{
		m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), m_GameObjects[i]));
	}

	void Scene::RemoveEmptyGameObjectUniquePointers()
	{
		m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), std::unique_ptr<GameObject>{}), m_GameObjects.end());
	}

}