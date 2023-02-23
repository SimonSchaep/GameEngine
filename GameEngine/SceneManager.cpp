#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Initialize()
{
	//prob shouldn't do this with all scenes
	for (auto& scene : m_scenes)
	{
		scene->Initialize();
	}
}

void dae::SceneManager::Update()
{
	//prob shouldn't do this with all scenes
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	//prob shouldn't do this with all scenes
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
