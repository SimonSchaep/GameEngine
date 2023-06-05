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
		Scene* CreateScene(const std::string& name, bool destroyAfterDisable);
		void SetActiveScene(Scene* pScene);
		void SetActiveSceneByName(const std::string& sceneName);

		Scene* GetActiveScene();
		Scene* GetSceneByName(const std::string& sceneName);

		void Initialize();
		void Update();
		void Render()const;
		void RenderUI();

		virtual ~SceneManager() override;
	private:
		friend class Singleton<SceneManager>;
		SceneManager();

		void SwitchDesiredAndActiveScene();

		std::vector<std::unique_ptr<Scene>> m_Scenes;
		Scene* m_pActiveScene{};
		Scene* m_pDesiredActiveScene{};

		bool m_IsInitialized{};
	};

}