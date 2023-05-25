#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace engine
{

	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);
		void SetActiveScene(Scene* pScene);
		void SetActiveSceneByName(const std::string& sceneName);
		void SetActiveSceneByIndex(int index);

		Scene* GetActiveScene();
		Scene* GetSceneByName(const std::string& sceneName);
		Scene* GetSceneByIndex(int index);

		void Initialize();
		void Update();
		void Render()const;
		void RenderUI();

		virtual ~SceneManager() override;
	private:
		friend class Singleton<SceneManager>;
		SceneManager();

		std::vector<std::unique_ptr<Scene>> m_Scenes;
		Scene* m_pActiveScene{};

		bool m_IsInitialized{};
	};

}