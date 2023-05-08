#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace engine
{
	class RenderComponent;

	class Scene final
	{
	public:
		GameObject* CreateAndAddGameObject(const std::string& name = "DefaultName", GameObject* pParent = nullptr);
		GameObject* AddGameObject(std::shared_ptr<GameObject> gameObject);
		GameObject* FindGameObjectByName(const std::string& name);
		std::shared_ptr<GameObject> GetSharedPtrForGameObject(GameObject* pGameObject);

		const std::vector<std::shared_ptr<GameObject>>& GetGameObjects()const { return m_GameObjects; }

		const std::string& GetName()const { return m_Name; };
		int GetIndex()const { return m_Index; };
		bool GetIsInitialized()const { return m_IsInitialized; };

		void Initialize();
		void Update();
		void Render()const;
		void RenderUI();

		~Scene();
		Scene(const std::string& name, int index);
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		void SortGameObjectsByRenderLayer();

		void RemoveGameObjectByIndex(size_t i);

		std::vector<size_t> m_ToDeleteIndexes{};

		std::string m_Name;
		int m_Index;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects{};

		bool m_IsInitialized{};
	};
}