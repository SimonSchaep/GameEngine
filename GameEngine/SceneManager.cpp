#include "SceneManager.h"
#include "Scene.h"
#include <assert.h>
#include <iostream>

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
		//set active scene to desired
		if (m_pDesiredActiveScene)
		{
			SwitchDesiredAndActiveScene();
		}

		//update
		if (m_pActiveScene)
		{
			m_pActiveScene->Update();
		}
	}

	void SceneManager::Render()const
	{
		if (m_pActiveScene && !m_pDesiredActiveScene) //don't render old scene anymore when new one was requested
		{
			m_pActiveScene->Render();
		}
	}

	void SceneManager::RenderUI()
	{
		if (m_pActiveScene && !m_pDesiredActiveScene) //don't render old scene anymore when new one was requested
		{
			m_pActiveScene->RenderUI();
		}
	}

	void SceneManager::ClearScenes()
	{
		m_Scenes.clear();
	}

	//needs to be here for the unique_ptrs of the scenes to compile
	SceneManager::~SceneManager() = default;
	SceneManager::SceneManager() = default;

	void SceneManager::SwitchDesiredAndActiveScene()
	{
		if (m_pActiveScene)
		{
			m_pActiveScene->TransferSceneIndependantGameObjects(m_pDesiredActiveScene);

			if (m_pActiveScene->GetShouldDestroyAfterDisable())
			{
				RemoveScene(m_pActiveScene);
			}
		}

		m_pActiveScene = m_pDesiredActiveScene;
		m_pDesiredActiveScene = nullptr;

		if (!m_pActiveScene->GetIsInitialized())
		{
			m_pActiveScene->Initialize();
		}
	}

	Scene* SceneManager::CreateScene(const std::string& name, bool destroyAfterDisable)
	{
		auto scene = std::make_unique<Scene>(name, destroyAfterDisable);
		Scene* pReturnValue = scene.get();
		m_Scenes.emplace_back(std::move(scene));
		if (!m_pActiveScene && !m_pDesiredActiveScene)
		{
			SetActiveScene(pReturnValue);
		}
		return pReturnValue;
	}

	void SceneManager::SetActiveScene(Scene* pScene)
	{
		assert(pScene);
		if(m_pActiveScene == pScene || m_pDesiredActiveScene == pScene) return;

		if (!m_pActiveScene)
		{
			m_pActiveScene = pScene;
		}
		else
		{
			//avoid switching scenes during update/initialize
			m_pDesiredActiveScene = pScene;
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

	void SceneManager::RemoveScene(Scene* pScene)
	{
		if (m_pActiveScene == pScene)m_pActiveScene = nullptr;

		m_Scenes.erase(std::remove_if(m_Scenes.begin(), m_Scenes.end(), [&](const std::unique_ptr<Scene>& scene)
			{
				return scene.get() == pScene;
			}));
	}

	void SceneManager::RemoveSceneByName(const std::string& sceneName)
	{
		RemoveScene(GetSceneByName(sceneName));
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
}