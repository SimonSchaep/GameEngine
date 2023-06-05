#include "Scene.h"
#include "RenderComponent.h"
#include "CameraComponent.h"
#include <algorithm>

namespace engine
{
	Scene::Scene(const std::string& name, int index) : m_Name(name), m_Index{index} {}

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
			if (gameObject->GetName() == name)
			{
				return gameObject.get();
			}
		}
		return nullptr;
	}

	void Scene::TransferSceneIndependantGameObjects(Scene* targetScene)
	{
		if(targetScene == this) return;

		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			if (m_GameObjects[i]->IsSceneIndependant())
			{
				targetScene->TransferGameObject(std::move(m_GameObjects[i]));
				m_ToDeleteIndexes.emplace_back(i);
			}
		}
		//Delete GameObjects - reverse order because it removes gameobjects by index
		//If the gameobjects at idx 0 and 1 need to be deleted and the one at 0 is deleted first, the gameobject at idx 1 will be at 0 when we delete it
		for (int i{ int(m_ToDeleteIndexes.size()) - 1 }; i >= 0; --i)
		{
			RemoveGameObjectByIndex(m_ToDeleteIndexes[i]);
		}
		m_ToDeleteIndexes.clear();
	}

	void Scene::TransferGameObject(std::unique_ptr<GameObject> gameObject)
	{
		m_GameObjects.emplace_back(std::move(gameObject));
	}

	void Scene::Initialize()
	{
		//no range-based for, cause than iterator would get invalidated if gameobjects get added during initailize
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

			m_GameObjects[i]->Update();
			if (m_GameObjects[i]->IsMarkedForDeletion())
			{
				m_ToDeleteIndexes.emplace_back(i);
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
				RenderComponent* pRenderComponent{ obj1->GetComponent<RenderComponent>() };
				if (pRenderComponent)
				{
					layer1 = pRenderComponent->GetLayer();
				}
				pRenderComponent = obj2->GetComponent<RenderComponent>();
				if (pRenderComponent)
				{
					layer2 = pRenderComponent->GetLayer();
				}
				return (layer1 < layer2);
			});
	}

	void Scene::RemoveGameObjectByIndex(size_t i)
	{
		m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), m_GameObjects[i]));
	}

}