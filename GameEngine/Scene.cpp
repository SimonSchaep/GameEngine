#include "Scene.h"

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Initialize()
{
	for (auto& object : m_objects)
	{
		object->Initialize();
	}
}

void Scene::Update()
{
	for (size_t i{}; i < m_objects.size(); ++i)
	{
		m_objects[i]->Update();
		if (m_objects[i]->IsMarkedForDeletion())
		{
			m_ToDeleteIndexes.push_back(i);
		}
	}

	//Delete objects
	for (size_t i : m_ToDeleteIndexes)
	{
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), m_objects[i]));
	}
	m_ToDeleteIndexes.clear();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

