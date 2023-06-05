#include "SceneManager.h"
#include "Scene.h"
#include <assert.h>

namespace engine
{

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

	//needs to be here for the unique_ptrs of the scenes to compile
	SceneManager::~SceneManager() = default;
	SceneManager::SceneManager() = default;

	Scene* SceneManager::CreateScene(const std::string& name)
	{
		auto scene = std::make_unique<Scene>(name, int(m_Scenes.size()));
		Scene* pReturnValue = scene.get();
		m_Scenes.emplace_back(std::move(scene));
		if (!m_pActiveScene)
		{
			SetActiveScene(pReturnValue);
		}
		return pReturnValue;
	}

	void SceneManager::SetActiveScene(Scene* pScene)
	{
		assert(pScene);
		if(m_pActiveScene == pScene) return;

		if (m_pActiveScene)
		{
			m_pActiveScene->TransferSceneIndependantGameObjects(pScene);
		}
		m_pActiveScene = pScene;
		if (m_IsInitialized && !m_pActiveScene->GetIsInitialized())
		{
			m_pActiveScene->Initialize();
		}
	}

	void SceneManager::SetActiveSceneByName(const std::string& sceneName)
	{
		for (auto& scene : m_Scenes)
		{
			if (scene->GetName() == sceneName)
			{
				SetActiveScene(scene.get());
			}
		}
	}

	void SceneManager::SetActiveSceneByIndex(int index)
	{
		SetActiveScene(GetSceneByIndex(index));
	}
	
	Scene* SceneManager::GetActiveScene()
	{
		return m_pActiveScene;
	}

	Scene* SceneManager::GetSceneByName(const std::string& sceneName)
	{
		for (auto& scene : m_Scenes)
		{
			if (scene->GetName() == sceneName)
			{
				return scene.get();
			}
		}
		return nullptr;
	}

	Scene* SceneManager::GetSceneByIndex(int index)
	{
		assert(int(m_Scenes.size()) > index);
		return m_Scenes[index].get();
	}

}