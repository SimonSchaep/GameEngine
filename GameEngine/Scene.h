#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace engine
{
	class RenderComponent;

	class Scene final
	{
	public:
		GameObject* CreateAndAddGameObject(const std::string& name = "DefaultName", GameObject* pParent = nullptr); //todo: add option to attach to parent immediately
		GameObject* FindGameObjectByName(const std::string& name);
		void RemoveAllGameObjects();

		void AddRenderComponent(RenderComponent* pRenderComponent){m_RenderComponents.push_back(pRenderComponent);}

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
		void RemoveGameObjectByIndex(size_t i);

		std::vector<size_t> m_ToDeleteIndexes{};

		std::string m_Name;
		int m_Index;
		std::vector <std::unique_ptr<GameObject>> m_GameObjects{};
		std::vector <RenderComponent*> m_RenderComponents{};

		bool m_IsInitialized{};
	};
}