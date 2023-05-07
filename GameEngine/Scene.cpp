#include "Scene.h"
#include "RenderComponent.h"
#include <algorithm>

namespace engine
{
	Scene::Scene(const std::string& name, int index) : m_Name(name), m_Index{index} {}

	Scene::~Scene() = default;

	GameObject* Scene::CreateAndAddGameObject(const std::string& name, GameObject* pParent)
	{
		auto gameObject = std::make_shared<GameObject>(name, m_Index);
		if (pParent)
		{
			gameObject->SetParent(pParent, false);
		}
		GameObject* pReturnValue = gameObject.get();
		m_GameObjects.emplace_back(std::move(gameObject));
		return pReturnValue;
	}

	GameObject* Scene::AddGameObject(std::shared_ptr<GameObject> gameObject)
	{
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

	void Scene::RemoveAllGameObjects()
	{
		m_GameObjects.clear();
	}

	std::shared_ptr<GameObject> Scene::GetSharedPtrForGameObject(GameObject* pGameObject)
	{
		for (auto& gameObject : m_GameObjects)
		{
			if (gameObject.get() == pGameObject)
			{
				return gameObject;
			}
		}
		return std::shared_ptr<GameObject>(nullptr);
	}

	void Scene::Initialize()
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->Initialize();
		}
		m_IsInitialized = true;
	}

	void Scene::Update()
	{
		//Update GameObjects
		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			m_GameObjects[i]->Update();
			if (m_GameObjects[i]->IsMarkedForDeletion())
			{
				m_ToDeleteIndexes.push_back(i);
			}
		}

		//Delete GameObjects - reverse order because it removes gameobjects by index
		//If the gameobjects at idx 0 and 1 need to be deleted and the one at 0 is deleted first, the gameobject at idx 1 will be at 0 when we delete it
		for (int i{ int(m_ToDeleteIndexes.size()) - 1 }; i >= 0; --i)
		{
			RemoveGameObjectByIndex(m_ToDeleteIndexes[i]);
		}
		m_ToDeleteIndexes.clear();


		//Sort rendercomponents
		std::sort(m_RenderComponents.begin(), m_RenderComponents.end(),
			[](RenderComponent* renderComp1, RenderComponent* renderComp2)
			{
				return (renderComp1->GetLayer() < renderComp2->GetLayer());
			});
	}

	void Scene::RenderUI()
	{
		for (const auto& gameObject : m_GameObjects)
		{
			gameObject->RenderUI();
		}
	}

	void Scene::Render()const
	{
		for (const auto& renderComponent : m_RenderComponents)
		{
			renderComponent->Render();
		}
	}

	void Scene::RemoveGameObjectByIndex(size_t i)
	{
		m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), m_GameObjects[i]));
	}

}