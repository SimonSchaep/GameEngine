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

	//Delete GameObjects
	for (size_t i : m_ToDeleteIndexes)
	{
		RemoveGameObjectByIndex(i);
	}
	m_ToDeleteIndexes.clear();
}

void Scene::Render() const
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
		pGameObject->SetParent(nullptr, false);
	}
	//if has children
	if (pGameObject->GetChildren().size() > 0)
	{
		//remove all children from parent
		for (auto child : pGameObject->GetChildren())
		{
			child->SetParent(nullptr, false);
		}
	}
	m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), m_GameObjects[i]));
}

