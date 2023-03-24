#include "Scene.h"

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

GameObject* Scene::CreateAndAddGameObject(const std::string& name)
{
	auto gameObject = std::make_unique<GameObject>(name);
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
	for (int i{ int(m_ToDeleteIndexes.size()) - 1}; i >= 0; --i)
	{
		RemoveGameObjectByIndex(m_ToDeleteIndexes[i]);
	}
	m_ToDeleteIndexes.clear();
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
	for (const auto& gameObject : m_GameObjects)
	{
		gameObject->Render();
	}
}

void Scene::RemoveGameObjectByIndex(size_t i)
{
	GameObject* pGameObject{ m_GameObjects[i].get() };
	//if has parent
	if (pGameObject->GetParent())
	{
		//remove from parent
		pGameObject->SetParent(nullptr, true);
	}
	//if has children
	if (pGameObject->GetChildren().size() > 0)
	{
		//remove all children from parent
		for (auto child : pGameObject->GetChildren())
		{
			child->SetParent(nullptr, true);
		}
	}
	m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), m_GameObjects[i]));
}

