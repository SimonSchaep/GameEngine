#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace engine
{
	class RenderComponent;
	class CameraComponent;

	class Scene final
	{
	public:
		GameObject* CreateAndAddGameObject(const std::string& name = "DefaultName", GameObject* pParent = nullptr);
		GameObject* FindGameObjectByName(const std::string& name);

		void TransferSceneIndependantGameObjects(Scene* targetScene);
		void TransferGameObject(std::unique_ptr<GameObject> gameObject);

		const std::vector<std::unique_ptr<GameObject>>& GetGameObjects()const { return m_GameObjects; }

		const std::string& GetName()const { return m_Name; };
		int GetShouldDestroyAfterDisable()const { return m_DestroyAfterDisable; };
		bool GetIsInitialized()const { return m_IsInitialized; };

		void Initialize();
		void Update();
		void Render()const;
		void RenderUI();

		void SetActiveCamera(CameraComponent* pCameraComponent) { m_pCameraComponent = pCameraComponent; }
		CameraComponent* GetActiveCamera() { return m_pCameraComponent; }

		~Scene();
		Scene(const std::string& name, bool destroyAfterDisable);
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		void SortGameObjectsByRenderLayer();

		void RemoveGameObjectByIndex(size_t i);
		void RemoveEmptyGameObjectUniquePointers();

		std::vector<size_t> m_ToDeleteIndexes{};

		std::string m_Name{};
		bool m_DestroyAfterDisable{};

		//todo: maybe use a vector of active and inactive gameobjects
		std::vector<std::unique_ptr<GameObject>> m_GameObjects{};

		bool m_IsInitialized{};

		CameraComponent* m_pCameraComponent{};
	};
}