#include "Scene.h"

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

GameObject* Scene::CreateAndAddGameObject()
{
	auto gameObject = std::make_unique<GameObject>();
	GameObject* pReturnValue = gameObject.get();
	m_GameObjects.emplace_back(std::move(gameObject));
	return pReturnValue;
}

void Scene::RemoveAll()
{
	m_GameObjects.clear();
}

void Scene::Initialize()
{
	for (auto& object : m_GameObjects)
	{
		object->Initialize();
	}
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
	for (const auto& object : m_GameObjects)
	{
		object->RenderUI();
	}
}

void Scene::Render()const
{
	for (const auto& object : m_GameObjects)
	{
		object->Render();
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

