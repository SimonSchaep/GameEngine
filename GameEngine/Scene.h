#pragma once
#include "SceneManager.h"
#include "GameObject.h"

class Scene final
{
public:
	GameObject* CreateAndAddGameObject(const std::string& name = "DefaultName"); //todo: add option to attach to parent immidiately
	GameObject* FindGameObjectByName(const std::string& name);
	void RemoveAllGameObjects();

	const std::string& GetName()const { return m_Name; };
	bool GetIsInitialized()const { return m_IsInitialized; };

	void Initialize();
	void Update();
	void Render()const;
	void RenderUI();

	~Scene();
	Scene(const std::string& name);
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private:
	void RemoveGameObjectByIndex(size_t i);

	std::vector<size_t> m_ToDeleteIndexes{};

	std::string m_Name;
	std::vector <std::unique_ptr<GameObject>> m_GameObjects{};

	static unsigned int m_IdCounter;

	bool m_IsInitialized{};
};
