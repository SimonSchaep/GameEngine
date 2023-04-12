#include "SceneManager.h"
#include "Scene.h"
#include <assert.h>

void SceneManager::Initialize()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Initialize();
	}	
	m_IsInitialized = true;
}

void SceneManager::Update()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Update();
	}
}

void SceneManager::Render()const
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Render();
	}
}

void SceneManager::RenderUI()
{
	if (m_pActiveScene)
	{
		m_pActiveScene->RenderUI();
	}
}

//needs to be here for the unique_ptr to incomplete type scene to compile
SceneManager::~SceneManager() = default;
SceneManager::SceneManager() = default;

Scene* SceneManager::CreateScene(const std::string& name)
{
	auto scene = std::make_unique<Scene>(name);
	Scene* pReturnValue = scene.get();
	m_scenes.push_back(std::move(scene));
	if (!m_pActiveScene)
	{
		SetActiveSceneByPtr(pReturnValue);
	}
	return pReturnValue;
}

void SceneManager::SetActiveSceneByPtr(Scene* pScene)
{
	assert(pScene);
	m_pActiveScene = pScene;
	if (m_IsInitialized && !m_pActiveScene->GetIsInitialized())
	{
		m_pActiveScene->Initialize();
	}
}

void SceneManager::SetActiveSceneByName(const std::string& sceneName)
{
	for (auto& scene : m_scenes)
	{
		if (scene->GetName() == sceneName)
		{
			m_pActiveScene = scene.get();
		}
	}

	//ensure there is an active scene
	assert(m_pActiveScene);

	if (m_IsInitialized && !m_pActiveScene->GetIsInitialized())
	{
		m_pActiveScene->Initialize();
	}
}

Scene* SceneManager::GetActiveScene()
{
	return m_pActiveScene;
}
